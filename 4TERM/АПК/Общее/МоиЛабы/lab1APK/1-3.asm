
data segment
    msg_sent db "Sent ", '$'
    msg_got db 0Ah, 0Dh, "Received ", '$'
    msg_error db "Error", 0Ah, 0Dh, '$'
    
    buf1 db '1'
    buf2 db 0
ends data

stck segment
    dw 128 dup(0)
ends stck

code segment

ASSUME ss:stck, ds:data, cs:code

out_str macro str
    mov ah, 09h
    lea dx, str
    int 21h
endm
    
main:
    mov ax, data
    mov ds, ax
    
    mov dx, 3fBh      ;LCR register adress
    mov al, 10000011b ;init com1 (DLAB=1, 8 bits in symbol, 1 stopbit, no paritet)  
	out dx, al
	
	mov dx, 3f8h
	mov al, 0Ch
	out dx, al        ;write DIM (bit/sec = 9600)
	
    mov dx, 3f9h
    xor al, al
    out dx, al        ;write DLL=0
    
    mov dx, 3fBh
    mov al, 00000011b   ;DLAB=0
    out dx, al          ;write to LCR
    
    out_str msg_sent    ;msg that byte sent
    
    ReadyTHRECheck:
    mov dx, 3fdh
    in al, dx
    test al, 00100000b  ;check 5-th bit of LSR (THRE)
    jz ReadyTHRECheck
    
    mov dx, 3F8h                    
    mov al, buf1
    out dx, al                   ; Send data to the COM1. 
    
    mov dx, 3FDh
    in al, dx
    test al, 00001110b           ; Check errors (check LSR)
    jnz error
    
    mov ah, 02h 
    mov dl, buf1
    int 21h                      ;output sent character
          
    waitDR:                      ; Wait 1st bit of LSR register (DR)
    mov dx, 2FDh                  
    in al, dx
    test al, 01h                 ; Check 1st bit.
    jz waitDR
    
    mov dx, 2f8h
    in al, dx                  ;read from com2
    mov buf2, al                 ;save byte to buf2
    
    out_str msg_got              ;msg that byte sent
    
    mov ah, 02h
    mov dl, buf2
    int 21h                      ;output received character
    
    finish:
    mov ax, 4c00h                ;exit to operating system.
    int 21h
    
    error:
    out_str msg_error    
    jmp finish    
    
ends code

end main 