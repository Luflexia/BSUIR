.MODEL tiny

.CODE

START:
    ; Инициализация сегментов данных
    mov ax, @data
    mov ds, ax
    mov es, ax

    ; Вызов функции парсинга командной строки
    call PARSE_CMD

    ; Подготовка счетчика цикла
    xor cx, cx
    mov cl, TIMES_TO_RUN

    ; Если количество запусков равно 0, выходим
    cmp cl, 0
    je EXIT

    ; Выделение места в стеке
    mov sp, PROGRAMM_LENGTH + 300h
    mov ah, 4ah
    STACK_SHIFT = PROGRAMM_LENGTH + 300h
    mov bx, STACK_SHIFT shr 4 + 1
    int 21h

    ; Заполнение структуры EPB
    mov ax, cs
    mov word ptr EPB + 4,   ax
    mov word ptr EPB + 8,   ax
    mov word ptr EPB + 0ch, ax

    ; Вывод сообщения о количестве запусков
    lea dx, TIMES_TO_RUN_STR
    mov ah, 9
    int 21h

RUN_PROGRAM_CYCLE:
    ; Выполнение программы
    mov ax, 4b00h
    lea dx, PROGRAM_NAME
    lea bx, EPB
    int 21h

    ; Обработка ошибок
    jc FATAL_ERROR

loop RUN_PROGRAM_CYCLE:

    ; Завершение программы
    int 20h

EXIT:
    ; Завершение программы
    mov ah, 4ch
    int 21h

FATAL_ERROR:
    ; Обработка ошибок выполнения программ
    mov ah, 9
    cmp al, 2
    je SET_FILE_NOT_FOUND
    cmp al, 5
    je SET_ACCESS_DENIED
    cmp al, 0bh
    je SET_NOT_VALID_FORMAT

SHOW_MESSAGE:
    ; Вывод сообщения
    int 21h
    jmp EXIT

SET_FILE_NOT_FOUND:
    ; Установка сообщения о ненайденном файле
    lea dx, FILE_NOT_FOUND_MESSAGE
    jmp SHOW_MESSAGE

SET_ACCESS_DENIED:
    ; Установка сообщения об отказе в доступе
    lea dx, ACCESS_DENIED_MESSAGE
    jmp SHOW_MESSAGE

SET_NOT_VALID_FORMAT:
    ; Установка сообщения о неверном формате
    lea dx, NOT_VALID_FORMAT_MESSAGE
    jmp SHOW_MESSAGE

PARSE_CMD proc
    ; Получение длины командной строки
    mov cl, [80h]
    cmp cl, 0
    je EMPTY_PARAMETER

    ; Инициализация индекса и вызов функции пропуска пробелов
    mov di, 82h
    call SKIP_SPACES

    ; Запись имени программы
    lea si, PROGRAM_NAME
    call WRITE_PARAMETR

    ; Пропуск пробелов
    call SKIP_SPACES

    ; Запись количества запусков
    lea si, TIMES_TO_RUN_STR
    call WRITE_PARAMETR
    call TO_DIGIT

    ; Пропуск пробелов
    lea si, PARAMETER_FOR_RUN_PROGRAM
    call SKIP_SPACES

    ; Уменьшение индекса и запись параметра запуска программы
    dec di
    WRITE_PARAMETER_FOR_RUN_PROGRAM:
        cmp [di], 0dh
        je EXIT_FROM_PARSE

        mov al, [di]
        mov [si], al

        inc si
        inc di
    jmp WRITE_PARAMETER_FOR_RUN_PROGRAM

    EXIT_FROM_PARSE:
    ret

EMPTY_PARAMETER:
    ; Обработка случая пустой командной строки
    mov ah, 9
    lea dx, EMPTY_PARAMETER_MESSAGE
    int 21h
    mov ah, 4ch
    int 21h
PARSE_CMD endp

WRITE_PARAMETR proc
     WRITE_CYCLE:
        ; Запись символов параметра
        mov al, [di]
        cmp al, 0
        je END_OF_WRITE
        cmp al, ' '
        je END_OF_WRITE
        cmp al, 9
        je END_OF_WRITE
        cmp al, 0dh
        je END_OF_WRITE
        mov [si], al

        inc di
        inc si
        jmp WRITE_CYCLE

    END_OF_WRITE:
    ret
WRITE_PARAMETR endp

SKIP_SPACES proc
    ; Пропуск пробелов
    dec di
    CYCLE:
        inc di
        cmp [di], 0dh
        je END_OF_SKIPPING
        cmp [di], 0
        je END_OF_SKIPPING
        cmp [di], ' '
        je CYCLE
        cmp [di], 9
        je CYCLE
    END_OF_SKIPPING:
    ret
SKIP_SPACES endp

TO_DIGIT proc
    ; Преобразование строки в число
    push di
    call IS_VALID_DIGIT

    lea di, TIMES_TO_RUN_STR
    xor bx, bx
    xor ax, ax
    mov bl, 10

    TO_DIGIT_CYCLE:
        mov cl, [di]
        sub cl, '0'

        mul bx
        add al, cl
        inc di
        cmp [di], '$'
    jne TO_DIGIT_CYCLE
    pop di
    mov TIMES_TO_RUN, al
    ret
TO_DIGIT endp

IS_VALID_DIGIT proc
    ; Проверка, является ли символ цифрой
    lea di, TIMES_TO_RUN_STR
    xor cx, cx
    mov cl, 4

    FIND_SIZE_CYCLE:
        cmp [di], '$'
        je EXIT_FROM_CYCLE
        cmp [di], '0'
        jl NOT_VALID_DIGIT
        cmp [di], '9'
        jg NOT_VALID_DIGIT

        inc di
    loop FIND_SIZE_CYCLE

    EXIT_FROM_CYCLE:

    lea di, TIMES_TO_RUN_STR
    cmp cl, 4
    je NOT_VALID_DIGIT

    cmp cl, 1
    jl NOT_VALID_DIGIT
    je IS_LOWER
    ret

IS_LOWER:
    ; Проверка, что число не превышает 255
    cmp [di], '2'
    jg NOT_VALID_DIGIT
    cmp [di + 1], '5'
    jg NOT_VALID_DIGIT
    cmp [di + 2], '5'
    jg NOT_VALID_DIGIT
    ret

NOT_VALID_DIGIT:
    ; Обработка ошибки неверного символа
    mov ah, 9
    lea dx, NOT_VALID_DIGIT_MESSAGE
    int 21h
    mov ah, 4ch
    int 21h

IS_VALID_DIGIT endp

LINE db 128 dup ('$')
TIMES_TO_RUN db 0
TIMES_TO_RUN_STR db 4 dup('$')
PROGRAM_NAME db 128 dup(0)
PARAMETER_FOR_RUN_PROGRAM 128 dup(0dh)

EPB dw 0000
    dw offset PARAMETER_FOR_RUN_PROGRAM
    dw 005ch, 0, 006ch, 0

NOT_VALID_DIGIT_MESSAGE db "Digit must be in [0, 255]$"
EMPTY_PARAMETER_MESSAGE db "You should enter name of program and quantity of starts$"
FILE_NOT_FOUND_MESSAGE db "Executable file not found$"
ACCESS_DENIED_MESSAGE db "Access denied$"
NOT_VALID_FORMAT_MESSAGE db "Not valid format message$"

PROGRAMM_LENGTH EQU $ - START
end START
