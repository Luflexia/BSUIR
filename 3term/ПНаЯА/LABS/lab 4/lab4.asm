.model small
.stack 100h
.data
    videobuffSize equ 0FA0h  ;25*80*2
    startPositionOffset dw 07CAh  ;Row 12, Column 37
    videobuff dw 0B800h
    videobuffHeight db 25
    videobuffLengthInBytes db 160
    
    defaultDirection equ right

    pointsUntilSpeedup equ 02h
    defaultInterruptsForMove equ 03h

    defaultSnakeLength equ 0005h
    maxSnakeLength equ 2000 
    
    snakeCell equ 0009h ; White circle
    point equ 0B03h ; Cyan heart 
            
    nTwo db 02h
       
    welcomText db "Snake!",0Dh,0Ah,"Control:",0Dh,0Ah,'$'
    controlText db 18h,", ",19h,", -> , <-",0Dh,0Ah,"Backspace - restart",0Dh,0Ah,"Esc - exit",0Dh,0Ah,'$' 
    startOrExitText db "Press any key to start, Esc to exit...$"
    
    gameOverText db "Game over! Final score: $"
    newLine db 0Dh,0Ah,'$'
    
    top equ 48h
    right equ 4Dh
    bottom equ 50h
    left equ 4Bh
    direction db defaultDirection
    
    maxFinalScoreDigits equ 5
    finalScore dw ?
    finalScoreString db maxFinalScoreDigits dup('$')
    pointPos dw ?     
    interruptsForMove db ?
    interruptsCounter db ?
    currentPointsUntilSpeedup db ?
    snakeLength dw 0000h
    snake dw maxSnakeLength dup(?)    ;Array of cell offsets
    
    snakeMoved db ?
    exit db 00h
    endGame db ?
    restart db ?
.code          
    
start:
    mov ax, data
    mov ds, ax
    mov es, ax    
    
    call clearScreen    

    mov ax, 351Ch   ;get timer handler
    int 21h
    mov cs:oldTimerHandler, bx        
    mov cs:oldTimerHandler + 2, es 
    
    mov ax, 3509h   ;get keyboard handler
    int 21h
    mov cs:oldKeyboardHandler, bx        
    mov cs:oldKeyboardHandler + 2, es
    
    mov ax, ds
    mov es, ax
       
    call showStartMessages
    mov ax, 0C08h 
    int 21h
    cmp al, 1Bh
    je closeApp  ;close if Esc cliked
startGame:    
    call clearScreen
    call buildStartState
    call outputStartElements
    
    cli  ;cleat interrupt flag = 0
    mov ax, cs
    mov ds, ax

    lea dx, newKeyboardHandler
    mov ax, 2509h
    int 21h
    lea dx, newTimerHandler
    mov ax, 251Ch
    int 21h
    
    mov ax, es
    mov ds, ax
    sti   ;set interrupt flag = 1
    
gameLoop:
    cmp endGame, 0
    je gameLoop
    
    cli
    mov ax, cs:oldTimerHandler + 2
    mov ds, ax
    
    mov dx, cs:oldTimerHandler
    mov ax, 251Ch
    int 21h
    
    mov ax, cs:oldKeyboardHandler + 2
    mov ds, ax
    mov dx, cs:oldKeyboardHandler
    mov ax, 2509h
    int 21h
    
    mov ax, es
    mov ds, ax
    sti
    
    cmp exit, 01h
    je closeApp
    cmp restart, 01h
    je startGame
    call showGameOverMessages
    mov ax, 0C08h
    int 21h
    cmp al, 1Bh
    jne startGame
    
closeApp:
    call clearScreen       
    mov ax, 4c00h ; exit to operating system.
    int 21h    
ends

clearScreen proc
    mov ax, 0002h
    int 10h
ret
clearScreen endp            

outputString proc
    mov     ah, 9
    int     21h
ret
outputString endp

showStartMessages proc
    lea dx, welcomText
    call outputString
    lea dx, controlText
    call outputString
    lea dx, startOrExitText
    call outputString
ret
showStartMessages endp

showGameOverMessages proc
    lea dx, gameOverText
    call outputString
    mov ax, finalScore 
    lea si, finalScoreString
    call numberToString
    mov dx, si
    call outputString
    lea dx, newLine
    call outputString
    lea dx, startOrExitText
    call outputString    
ret
showGameOverMessages endp

buildStartState proc
    cmp snakeLength, 0  ;create snake
    je buildNewSnake
    lea si, snake
    mov cx, snakeLength
    xor ax, ax
resetSnake:
    mov [si], ax
    add si, 2
    loop resetSnake
buildNewSnake:    
    mov snakeLength, defaultSnakeLength
    mov cx, defaultSnakeLength
    lea si, snake
    mov ax, startPositionOffset
buildSnake:
    mov [si], ax
    add si, 2
    add ax, 2
    loop buildSnake
randomPointPos:        
    call random
    cmp bx, startPositionOffset
    jl savePointPos
    sub ax, 2
    cmp bx, ax
    jg savePointPos
    shr bx, 1
savePointPos:
    mov cx, maxFinalScoreDigits
    lea si, finalScoreString
clearScoreBuffer:
    mov [si], '$'
    inc si
    loop clearScoreBuffer
    
    xor ax, ax    
    mov pointPos, bx
    mov currentPointsUntilSpeedup, pointsUntilSpeedup
    mov interruptsCounter, al
    mov interruptsForMove, defaultInterruptsForMove
    mov endGame, al
    mov restart, al
    mov direction, defaultDirection
    mov snakeMoved, al
    mov finalScore, ax
ret
buildStartState endp

;ax - source number, si - result buffer
numberToString proc
    pusha
    xor cx, cx
    mov bx, 000Ah
numberToStringParse:    
    xor dx, dx
    div bx
    add dx, '0'
    inc cx
    push dx
    cmp ax, 0
    jne numberToStringParse
moveNumberToString:
    pop ax
    mov [si], al    
    inc si
    loop moveNumberToString
    popa
ret
numberToString endp

;bx - number between 0 and 3999
random proc
    push ax
    push dx
    push cx
    mov ah, 2Ch
    int 21h
    xor ax, ax
    mov al, dl
    mul dh
    test ax, 1
    jz checkNumber
    inc ax
checkNumber:    
    cmp ax, videobuffSize
    jl setPointPos
    shr ax, 1
    test ax, 1
    jz setPointPos
    inc ax
setPointPos:
    mov bx, ax
    pop cx
    pop dx
    pop ax     
ret    
random endp

outputStartElements proc
    mov ax, videobuff
    mov ds, ax
    lea di, snake
outputCycle:
    mov si, es:di
    cmp si, 0
    je outputEnd
    mov [si], snakeCell
    add di, 2
    jmp outputCycle
outputEnd:
    lea di, pointPos
    mov si, es:di
    mov [si], point
    mov ax, es
    mov ds, ax
ret
outputStartElements endp


newTimerHandler proc far
    cmp endGame, 01h
    je timerOldInt
    cmp interruptsForMove, 01h
    je move    
    inc interruptsCounter
    mov cl, interruptsCounter
    cmp cl, interruptsForMove
    jne timerOldInt
    mov interruptsCounter, 00h

move:
    mov snakeMoved, 01h    
    lea si, snake
    mov ax, snakeLength
    dec ax
    mul nTwo
    add si, ax
    mov ax, [si]
    
    mov dl, direction
    cmp dl, top
    jne moveRight
    sub ax, 160     ;One line size
    cmp ax, 0
    jnl isCrash
    add ax, videobuffSize
    jmp isCrash
          
moveRight:
    cmp dl, right
    jne moveBottom
    mov bx, ax      ;One cell size
    add bx, 2
    div videobuffLengthInBytes
    mov dx, ax
    mov ax, bx
    div videobuffLengthInBytes
    cmp al, dl
    mov ax, bx
    je isCrash
    sub ax, 160
    jmp isCrash 
moveBottom:    
    cmp dl, bottom
    jne moveLeft
    add ax, 160
    cmp ax, videobuffSize
    jl isCrash
    sub ax, videobuffSize    
    jmp isCrash 
moveLeft:    
    mov bx, ax      ;One cell size
    sub bx, 2
    cmp bx, 0
    jge checkLeftBorder
    mov ax, 158
    jmp isCrash
checkLeftBorder:    
    div videobuffLengthInBytes
    mov dx, ax
    mov ax, bx
    div videobuffLengthInBytes
    cmp al, dl
    mov ax, bx
    je isCrash
    add ax, 160 
    
isCrash:
    lea si, snake
    mov cx, snakeLength
isCrashCycle:
    mov bx, [si]
    cmp bx, ax
    je crash
    add si, 2
    loop isCrashCycle
    jmp isPoint         
crash:
    mov endGame, 01h 
    jmp timerOldInt    
    
isPoint:
    cmp ax, pointPos
    je addNewCell
    lea di, snake
deleteLastCell:    
    mov bx, [di]
    mov dx, videobuff
    mov ds, dx
    mov si, bx
    mov [si], 0000h
    mov dx, es
    mov ds, dx
   
    mov si, di
    mov cx, snakeLength
    dec cx
moveBuffCycle: 
    add di, 2
    mov bx, [di]
    mov [si], bx
    add si, 2
    loop moveBuffCycle
moveLastCell:
    mov [si], ax
    mov dx, videobuff
    mov ds, dx    
    jmp moveSnake  
    
addNewCell:
    mov [si], ax
    inc finalScore
    inc snakeLength 
    
    cmp interruptsForMove, 01h
    je randomNewPoint
    dec currentPointsUntilSpeedup
    mov bl, currentPointsUntilSpeedup
    cmp bl, 0
    jne randomNewPoint
    dec interruptsForMove
    mov currentPointsUntilSpeedup, pointsUntilSpeedup 
    
randomNewPoint:    
    call random 
    lea si, snake
    mov cx, snakeLength
checkPointPos:
    cmp [si], bx
    je changePoint
    add si, 2
    loop checkPointPos
    jmp printNewPoint 
changePoint:
    mov dx, videobuffSize
    sub dx, 2
    sub dx, bx
    shr dx, 1
    test dx, 1      
    jz isEven
    inc dx
isEven:
    mov bx, dx
    lea si, snake
    mov cx, snakeLength
    jmp checkPointPos    

printNewPoint:
    mov pointPos, bx
    mov dx, videobuff
    mov ds, dx
    mov si, bx
    mov [si], point    
    
moveSnake:
    mov si, ax
    mov [si], snakeCell
    
    mov ax, es
    mov ds, ax             
timerOldInt:    
    db 0EAh
    oldTimerHandler dd ?  
newTimerHandler endp

newKeyboardHandler proc far
    in al, 60h
    cmp al, 01h  ;Esc
    je endGamePressed
    cmp al, 0Eh  ;Backspace
    je restartGamePressed
    
    cmp snakeMoved, 00h
    je keyboardOldInt
    cmp al, top
    jne rightKey
    cmp direction, bottom
    je keyboardOldInt
    mov direction, top
    jmp changeDirectionPressed
    
rightKey:
    cmp al, right
    jne bottomKey
    cmp direction, left
    je keyboardOldInt
    mov direction, right
    jmp changeDirectionPressed
    
bottomKey:
    cmp al, bottom
    jne leftKey
    cmp direction, top
    je keyboardOldInt
    mov direction, bottom
    jmp changeDirectionPressed
        
leftKey:
    cmp al, left
    jne keyboardOldInt
    cmp direction, right
    je keyboardOldInt
    mov direction, left
    jmp changeDirectionPressed     

changeDirectionPressed:
    mov snakeMoved, 00h
    jmp keyboardOldInt    

restartGamePressed:
    mov restart, 01h
    mov endGame, 01h
    jmp keyboardOldInt    
endGamePressed:
    mov endGame, 01h
    mov exit, 01h
    
keyboardOldInt:        
    db 0EAh    
    oldKeyboardHandler dd ?
newKeyboardHandler endp

end start 