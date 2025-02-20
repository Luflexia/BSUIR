.model small
.stack 100h
.data
Error_Write db "Write error!",0Dh,0Ah,'$'
Error_Read db "Read error!",0Dh,0Ah,'$'
Information db "Byte sent: $"

.code
jmp start

Init_COM1 proc
   xor ax,ax
   mov al,10100011b ; Устанавливаем параметры порта COM1 (9600 бит/с, 8 бит данных, 1 стоп-бит, без проверки четности)
   mov dx,0
   int 14h
   ret            
Init_COM1 endp

IsWrite_COM1 proc
   mov al,'A' ; Пытаемся записать символ 'A' в порт COM1
   mov ah,1
   mov dx,0
   int 14h
   test al,80h ; Проверяем флаг успешной записи
   jnz NoWrite ; Если флаг установлен, переходим к обработке ошибки записи
   ret 
IsWrite_COM1 endp

NoWrite proc
   mov ah,9
   mov dx,offset Error_Write ; Выводим сообщение об ошибке за-писи
   add dx,2
   int 21h
   ret 
NoWrite endp

IsRead_COM2 proc
    mov ah,2 ; Читаем символ из порта COM2
    mov dx,1
    int 14h
    test al,80h ; Проверяем флаг успешного чтения
    jnz NoRead ; Если флаг установлен, переходим к обработке ошибки чтения
    ret
IsRead_COM2 endp

NoRead proc
   mov ah,9
   mov dx,offset Error_Read ; Выводим сообщение об ошибке чте-ния
   add dx,2
   int 21h
   ret 
NoRead endp

Output proc
   mov ah,02h ; Выводим символ в стандартный вывод
   mov dl,al
   int 21h
   ret
Output endp

Exit proc
    mov ax,4C00h ; Завершаем программу
    int 21h
    ret
Exit endp

start:
   call Init_COM1 ; Инициализируем порт COM1
   call IsWrite_COM1 ; Пытаемся записать символ 'B' в порт COM1
   mov al,'B' ; Читаем символ из порта COM2
   call IsRead_COM2
   call Output ; Выводим прочитанный символ
   call Exit
end start
