import random 
import codecs

INITIAL_VALUE = 0x00
POLYNOM = 0x29

def introduce_error(data, probability=0.4):
    if random.random() <= probability:
        byte_array = bytearray(data.encode('utf-8'))
        byte_index = random.randint(0, len(byte_array) - 1)
        bit_index = random.randint(0, 7)
        byte_array[byte_index] ^= (1 << bit_index)
        data = byte_array.decode('utf-8', errors='ignore')
    return data


def compute_crc8(data: str) -> int:
    remainder = INITIAL_VALUE   
    bytes_data = codecs.encode(data, 'utf-8')

    for byte in bytes_data:
        remainder ^= byte
        for _ in range(8):
            remainder <<= 1
            if (remainder & 0x100) != 0:
                remainder ^= (POLYNOM | 0x100)

    return remainder & 0xF


def cyclic_shift_correction(message: str, crc: int) -> str:
    weight = bin(crc).count('1')
    shift_message = bytearray(message, 'utf-8')

    if weight <= 1:
        shift_message[-1] ^= crc
        return shift_message.decode('utf-8')

    count_shifts = 0
    size_bits = len(shift_message) * 8

    for _ in range(size_bits):
        count_shifts += 1
        shift_message = left_cycle_shift(shift_message)
        crc = compute_crc8(shift_message.decode('utf-8'))

        if bin(crc).count('1') <= 1:
            shift_message[-1] ^= crc

            for _ in range(count_shifts):
                shift_message = right_cycle_shift(shift_message)

            return shift_message.decode('utf-8')

    return message


def right_cycle_shift(byte_message: bytearray) -> bytearray:
    shift_byte_message = bytearray(len(byte_message))
    lowest_bit_of_last_byte = byte_message[-1] & 1

    for i in range(len(byte_message)):
        shift_byte_message[i] = (byte_message[i] & 0xFF) >> 1
        if i > 0:
            shift_byte_message[i] |= (byte_message[i - 1] & 1) << 7

    shift_byte_message[0] |= lowest_bit_of_last_byte << 7
    return shift_byte_message


def left_cycle_shift(byte_message: bytearray) -> bytearray:
    shift_byte_message = bytearray(len(byte_message))
    highest_bit_of_first_byte = (byte_message[0] & 0x80) >> 7

    for i in range(len(byte_message)):
        shift_byte_message[i] = ((byte_message[i] & 0xFF) << 1) & 0xFF
        if i < len(byte_message) - 1:
            shift_byte_message[i] |= (byte_message[i + 1] & 0x80) >> 7

    shift_byte_message[-1] |= highest_bit_of_first_byte
    return shift_byte_message
