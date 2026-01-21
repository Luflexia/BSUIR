import socket
import struct
import threading
import signal
import time
import sys
import random
from threading import Event, Lock

# Global event for graceful shutdown
stop_event = Event()
# Lock for thread-safe socket sending and printing
send_lock = Lock()
print_lock = Lock()

def signal_handler(sig, frame):
    """Handle Ctrl+C to stop all threads gracefully."""
    print('\nStopping all threads...')
    stop_event.set()

signal.signal(signal.SIGINT, signal_handler)

def checksum(data):
    """Calculate the checksum for ICMP or IP headers."""
    if len(data) % 2 == 1:
        data += b'\x00'
    s = 0
    for i in range(0, len(data), 2):
        s += (data[i] << 8) + data[i + 1]
    s = (s >> 16) + (s & 0xffff)
    s += (s >> 16)
    return ~s & 0xffff

def create_ip_header(source, dest, ttl, total_len, ident=12345):
    """Create an IP header with the specified parameters."""
    ip_header = struct.pack('!BBHHHBBH4s4s',
                            69,  # Version 4, IHL 5 (20 bytes)
                            0,   # Type of Service
                            total_len,  # Total length
                            ident,      # Identification
                            0x4000,     # Flags: Don't Fragment
                            ttl,        # Time to Live
                            1,          # Protocol: ICMP
                            0,          # Checksum placeholder
                            socket.inet_aton(source),
                            socket.inet_aton(dest))
    chksum = checksum(ip_header)
    ip_header = struct.pack('!BBHHHBBH4s4s',
                            69, 0, total_len, ident, 0x4000, ttl, 1, chksum,
                            socket.inet_aton(source),
                            socket.inet_aton(dest))
    return ip_header

def create_icmp_echo(id, seq, timestamp):
    """Create an ICMP Echo Request packet with timestamp in payload."""
    data = struct.pack('!d', timestamp)  # 8-byte timestamp
    # Pad data to a reasonable size (e.g., 56 bytes total packet - 8 bytes header)
    data += b'\x00' * (48 - len(data))
    icmp_header = struct.pack('!BBHHH', 8, 0, 0, id, seq) + data
    chksum = checksum(icmp_header)
    icmp_header = struct.pack('!BBHHH', 8, 0, chksum, id, seq) + data
    return icmp_header

def parse_ip_packet(packet):
    """Parse IP packet to extract source IP, protocol, and ICMP data."""
    if len(packet) < 20:
        return None, None, None
    ihl = packet[0] & 0x0F
    if ihl < 5:
        return None, None, None
    ip_header_len = ihl * 4
    src = socket.inet_ntoa(packet[12:16])
    protocol = packet[9]
    icmp_data = packet[ip_header_len:]
    return src, protocol, icmp_data

def parse_icmp_response(icmp_data, dest_ip, my_id, seq, sent_time):
    """Parse ICMP response and verify it matches the request."""
    if len(icmp_data) < 8:
        return None
    icmp_type, code = struct.unpack('!BB', icmp_data[0:2])
    if icmp_type == 0 and code == 0:  # Echo Reply
        pid, pseq = struct.unpack('!HH', icmp_data[4:8])
        if pid == my_id and pseq == seq:
            recv_time = time.time()
            if len(icmp_data) >= 16:
                timestamp = struct.unpack('!d', icmp_data[8:16])[0]
                rtt = recv_time - timestamp
                return ('reply', 'Echo Reply', rtt)
    elif icmp_type == 11 and code == 0:  # Time Exceeded
        if len(icmp_data) < 8 + 20 + 8:
            return None
        q_ip_start = 8
        q_ihl = icmp_data[q_ip_start] & 0x0F
        q_ip_len = q_ihl * 4
        q_dest_start = q_ip_start + 16
        q_dest = socket.inet_ntoa(icmp_data[q_dest_start:q_dest_start + 4])
        q_icmp_start = q_ip_start + q_ip_len
        q_type = icmp_data[q_icmp_start]
        if q_type == 8:
            q_id, q_seq = struct.unpack('!HH', icmp_data[q_icmp_start + 4:q_icmp_start + 8])
            if q_dest == dest_ip and q_id == my_id and q_seq == seq:
                rtt = time.time() - sent_time
                return ('ttl_exceeded', 'Time Exceeded', rtt)
    elif icmp_type == 3:  # Destination Unreachable
        unreachable_types = {0: 'Network Unreachable', 1: 'Host Unreachable', 3: 'Port Unreachable', 4: 'Fragmentation Needed'}
        msg = unreachable_types.get(code, f'Unreachable Code {code}')
        if len(icmp_data) < 8 + 20 + 8:
            return None
        q_ip_start = 8
        q_ihl = icmp_data[q_ip_start] & 0x0F
        q_ip_len = q_ihl * 4
        q_dest_start = q_ip_start + 16
        q_dest = socket.inet_ntoa(icmp_data[q_dest_start:q_dest_start + 4])
        q_icmp_start = q_ip_start + q_ip_len
        q_type = icmp_data[q_icmp_start]
        if q_type == 8:
            q_id, q_seq = struct.unpack('!HH', icmp_data[q_icmp_start + 4:q_icmp_start + 8])
            if q_dest == dest_ip and q_id == my_id and q_seq == seq:
                rtt = time.time() - sent_time
                return ('unreachable', msg, rtt)
    return None

def traceroute_host(host, shared_sock, max_hops=30, timeout=3.0):
    """Perform traceroute to the specified host using a shared socket."""
    my_id = random.randint(1, 65535)  # Unique ID per thread
    try:
        dest_ip = socket.gethostbyname(host)
        # Get source IP
        tmp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        tmp_sock.connect((dest_ip, 33434))
        source_ip = tmp_sock.getsockname()[0]
        tmp_sock.close()
        with print_lock:
            print(f"\nTraceroute to {host} ({dest_ip}) [Thread ID: {my_id}]")
        hop = 1
        while hop <= max_hops and not stop_event.is_set():
            sent_time = time.time()
            icmp_packet = create_icmp_echo(my_id, hop, sent_time)
            total_len = 20 + len(icmp_packet)
            ip_header = create_ip_header(source_ip, dest_ip, hop, total_len)
            packet = ip_header + icmp_packet
            with send_lock:
                shared_sock.sendto(packet, (dest_ip, 0))
            probe_start = time.time()
            response_received = False
            while time.time() - probe_start < timeout and not response_received and not stop_event.is_set():
                try:
                    # Use MSG_PEEK to inspect packet without consuming it
                    peeked, addr = shared_sock.recvfrom(4096, socket.MSG_PEEK)
                    src, proto, icmp = parse_ip_packet(peeked)
                    if proto == 1:  # ICMP protocol
                        result = parse_icmp_response(icmp, dest_ip, my_id, hop, sent_time)
                        if result:
                            # Consume the packet since it matches
                            shared_sock.recvfrom(4096)
                            typ, msg, rtt = result
                            with print_lock:
                                if typ == 'reply':
                                    print(f"{hop:2}  {src:15}  {rtt*1000:6.1f} ms ({msg})")
                                    response_received = True
                                    break
                                elif typ == 'ttl_exceeded':
                                    print(f"{hop:2}  {src:15}  {rtt*1000:6.1f} ms ({msg})")
                                    response_received = True
                                    break
                                elif typ == 'unreachable':
                                    print(f"{hop:2}  {src:15}  {msg} ({rtt*1000:6.1f} ms)")
                                    return
                except socket.timeout:
                    pass
                except socket.error as e:
                    if e.errno not in (socket.EWOULDBLOCK, socket.EAGAIN, socket.EINTR):
                        raise
                time.sleep(0.001)  # Prevent busy waiting
            if not response_received:
                with print_lock:
                    print(f"{hop:2}  {'*':15}  {'Timeout':6}")
            if typ == 'reply':
                break
            hop += 1
        with print_lock:
            print(f"Traceroute to {host} completed.")
    except Exception as e:
        with print_lock:
            print(f"Error during traceroute to {host}: {e}")

def main():
    if len(sys.argv) < 2:
        print("Usage: sudo python main.py <host1> [host2 host3 ...]")
        print("Note: Requires root privileges for raw sockets.")
        sys.exit(1)
    hosts = sys.argv[1:]
    try:
        # Create a single shared raw socket
        shared_sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_ICMP)
        shared_sock.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
        shared_sock.settimeout(0.1)  # Small timeout for non-blocking peeks
        threads = []
        for host in hosts:
            thread = threading.Thread(target=traceroute_host, args=(host, shared_sock))
            thread.daemon = True
            thread.start()
            threads.append(thread)
        # Keep main thread alive until all threads complete or interrupted
        for thread in threads:
            thread.join()
    except PermissionError:
        print("Error: This program requires root privileges to create raw sockets.")
        sys.exit(1)
    except Exception as e:
        print(f"Socket creation error: {e}")
    finally:
        if 'shared_sock' in locals():
            shared_sock.close()

if __name__ == "__main__":
    main()
