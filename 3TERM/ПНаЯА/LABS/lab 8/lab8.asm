.model tiny
    
.code 
org 100h 

START:
    ; Инициализация сегментов данных
    mov ax, cs
    mov ds, ax
    mov es, ax         
                              
    ; Проверка параметра
    mov bx, 80h
    cmp [bx], 0
    je WRONG_PARAMETR        
                     
    ; Получение имени файла
    call GET_FILE_NAME     
    ; Проверка, является ли файл текстовым
    call IS_TXT_FILE      
    ; Установка нового обработчика прерываний
    call INSTALL_NEW_HANDLER
    ; Открытие файла
    call OPEN_FILE
    ; Очистка экрана
    call CLEAR   
    ; Вывод сообщения о начале программы
    call SHOW_MESSAGE
 
    for0:
        ; Чтение из файла
        call READ_FROM_FILE
        ; Вывод содержимого буфера
        call OUTPUT_BUFFER
    jmp for0
    
EXIT:     
    ; Восстановление старого обработчика прерываний
    call INSTALL_OLD_HANDLER
    ; Закрытие файла
    call CLOSE_FILE    
    ; Завершение программы
    mov ah, 4ch
    int 21h           
    
WRONG_PARAMETR:
    ; Обработка ошибки неверного параметра
    lea dx, PARAMETR_ERROR
    mov ah, 9
    int 21h
    mov ah,  4ch
    int 21h    

SHOW_MESSAGE proc    
    ; Вывод сообщения на экран
    mov ax, 0b800h
    mov es, ax
    lea di, MESSAGE_TO_EXIT
    xor bx, bx       
    mov dh, 00000100b
    
    MESSAGE_CYCLE:
        mov dl, [di]    
        mov es:[bx], dx
        add bx, 2
        inc di
        cmp [di], '$'
    jne MESSAGE_CYCLE 
    
    ; Заполнение оставшегося пространства символами пробела
    mov dl, ' '
    
    FILL_STRING_CYCLE:
        mov es:[bx], dx
        add bx, 2
        cmp bx, 162
    jne FILL_STRING_CYCLE
    
    ret    
SHOW_MESSAGE endp

GET_FILE_NAME proc       
    ; Получение имени файла из командной строки
    mov di, 81h
    lea si, FILE_NAME
    
    mov al, ' '
    repe scasb     
    dec di         
    
    WRITE_CYCLE:
        mov ax, [di]
        mov [si], ax
        inc di
        inc si
        cmp [di], 0dh
    jne WRITE_CYCLE    
    mov [si], 0          
    
    ; Вывод имени файла на экран
    lea dx, FILE_NAME
    mov ah, 9
    int 21h
    
    ret
GET_FILE_NAME endp 

IS_TXT_FILE proc   
    ; Проверка, является ли файл текстовым (.txt)
    sub si, 4
    lea di, TXT_STRING          
    mov cx, 4
    repe cmpsb
    jne WRONG_PARAMETR    
    ret
IS_TXT_FILE endp

READ_FROM_FILE proc
    ; Чтение из файла в буфер
    mov ah, 3fh
    mov bx, FILE_DESKRIPTOR
    xor cx, cx
    mov cl, BUFFER_SIZE       
    lea dx, BUFFER
    int 21h               
    cmp al, 0 
    je EXIT
    mov BYTES_WAS_READ, al
    ret
READ_FROM_FILE endp

OUTPUT_BUFFER proc
    ; Вывод содержимого буфера на экран
    xor cx, cx
    mov cl, BYTES_WAS_READ
    lea di, BUFFER        
    
    mov ax, 0b800h
    mov es, ax
    mov dh, 00000111b
    mov bx, FILE_POINTER
    
    FOR1:
        mov dl, [di]
        mov es:[bx], dx
        add bx, 2
        inc di
        
        push cx
        mov cx, 0fffh
        
        cmp bx, 4000
        jl PAUSE
        
        ; Очистка экрана после каждых 25 строк
        call CLEAR
        mov bx, 160
        
        PAUSE:        
           cmp CLOSE, 1
           je EXIT
           cmp STOP, 1
           je PAUSE
        loop pause  
        pop cx            
        
        ; Замена символов перевода строки на пробелы
        cmp [di], 13
        je REPLACE_ON_SPACE
        cmp [di], 10
        jne END_OF_LOOP
        
        REPLACE_ON_SPACE:
        mov [di], ' '      
               
        END_OF_LOOP:
    loop FOR1
    mov FILE_POINTER, bx
    ret     
OUTPUT_BUFFER endp   
                                                                                                                                      
CLEAR_ALL_SCREEN proc      
    ; Очистка всего экрана
    call CLEAR             
    xor bx, bx
    mov cx, 80
    clear_cycle1:
        mov es:[bx], dx
        add bx, 2
    loop clear_cycle1
    ret
CLEAR_ALL_SCREEN endp                                                                                                                                          
                                                                                                                                      
CLEAR proc
    ; Очистка экрана
    mov ax, 0b800h
    mov es, ax
    mov bx, 160
    mov cx, 2000
    mov dl, ' '
    clear_cycle:
        mov dh, 00000111b
        mov es:[bx], dx
        add bx, 2    
    loop clear_cycle
    ret
CLEAR endp

OPEN_FILE proc 
    ; Открытие файла
    mov ah, 3dh
    mov al, 0
    lea dx, FILE_NAME
    int 21h
    mov FILE_DESKRIPTOR, ax
    jc ERROR
    ret
ERROR:
    ; Обработка ошибки открытия файла
    lea dx, OPEN_FILE_ERROR
    mov ah, 9
    int 21h
    jmp EXIT
OPEN_FILE endp

CLOSE_FILE proc
    ; Закрытие файла
    mov ah, 3Eh
    mov bx, FILE_DESKRIPTOR
    int 21h
    ret
CLOSE_FILE endp

NEW_INT9 proc far
    ; Обработчик прерывания 9
    push es
    in al, 60h         
    
    cmp al, 0AEh
    je EOI
    
    cmp al, 9DH
    je EOI
    
    cmp al, 1
    je SET_EXIT_FLAG    
    
    cmp al, 2Eh
    jne RESET
    
CTRL_IS_PRESSED:
    cmp PREW_SCAN_CODE, 1Dh
    jne RESET
    int 1bh         
    jmp EOI                 

SET_EXIT_FLAG:
    mov cs:CLOSE, 1
    jmp EOI
    
RESET:
    mov cs:STOP, 0  
    
EOI:
    mov PREW_SCAN_CODE, al
    mov al, 20h
    out 20h, al
    pop es
    iret
NEW_INT9 endp  

NEW_INT1B proc
    ; Обработчик прерывания 1B
    mov cs:STOP, 1
    iret
NEW_INT1b endp

INSTALL_NEW_HANDLER proc 
    ; Установка нового обработчика прерываний
    mov ax, 3509h
    int 21h
    mov word ptr cs:OLD_INT9 + 2, es
    mov word ptr cs:OLD_INT9, bx
    
    mov ax, 351Bh
    int 21h
    mov word ptr cs:OLD_INT1B + 2, es
    mov word ptr cs:OLD_INT1B, bx
    
    push cs
    pop ds
    
    mov ax, 2509h
    lea dx, NEW_INT9
    int 21h    
    
    mov ax, 251Bh
    lea dx, NEW_INT1B
    int 21h      
    ret
INSTALL_NEW_HANDLER endp

INSTALL_OLD_HANDLER proc
    ; Восстановление старого обработчика прерываний
    mov dx, word ptr cs:OLD_INT9
    mov ds, word ptr cs:OLD_INT9 + 2
    mov ax, 2509h
    int 21h
    
    mov dx, word ptr cs:OLD_INT1B
    mov ds, word ptr cs:OLD_INT1B + 2
    mov ax, 251Bh
    int 21h
    
    ret
INSTALL_OLD_HANDLER endp

OLD_INT9 dd ?
OLD_INT1B dd ?

STOP db 0
CLOSE db 0        
PREW_SCAN_CODE db 0
FILE_POINTER dw 160
FILE_DESKRIPTOR dw 0
COUNTER db 0
           
FILE_NAME db 50 dup('$')        
TXT_STRING db ".txt", 0  

BUFFER db 100 dup('$')                    
BUFFER_SIZE db 100
BYTES_WAS_READ db 0

OPEN_FILE_ERROR db "ERROR WITH OPENING FILE$"   
PARAMETR_ERROR db "PARAMETR MUST BE NAME OF TXT FILE$"       
MESSAGE_TO_EXIT db "PRESS ESC TO EXIT FROM PROGRAM$"

end start
