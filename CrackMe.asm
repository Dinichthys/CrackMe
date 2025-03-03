.model tiny
.186
locals ll
.code
org 100h

INPUT_MAX_LEN   equ 0Dh
STACK_ARRAY_LEN equ 05h

;-------------MAIN----------------

Main:
    call PrintHello

    call CheckPassword

    mov ax, 4c00h
    int 21h

;---------------------------------


;-----------FUNCTIONS-------------


;---------------------------------
; Print Massage pointed by dx
;
; Entry:  DX
; Exit:   None
; Destrs: AH, DS
;---------------------------------

PrintMassage proc

    mov ah, 09h
    push cs
    pop ds
    int 21h

    ret

endp

;---------------------------------


;---------------------------------
; Print Hello Massage
;
; Entry:  HelloMassage
; Exit:   None
; Destrs: AH, DS
;---------------------------------

PrintHello proc

    mov dx, offset HelloMassage
    call PrintMassage

    ret

endp

;---------------------------------


;---------------------------------
; Print Correct Result
;
; Entry:  CorrectResult
; Exit:   None
; Destrs: AH, DS
;---------------------------------

PrintCorrect proc

    mov dx, offset CorrectResult
    call PrintMassage

    ret

endp

;---------------------------------


;---------------------------------
; Print Incorrect Result
;
; Entry:  IncorrectResult
; Exit:   None
; Destrs: AH, DS
;---------------------------------

PrintIncorrect proc

    mov dx, offset IncorrectResult
    call PrintMassage

    ret

endp

;---------------------------------

;---------------------------------
; Print Hello Massage
;
; Entry:  HelloMassage
; Exit:   None
; Destrs: AH, DX, DS
;---------------------------------

CheckPassword proc

    call GetPassword

    push cs
    pop ds

    push ss
    pop es

    mov si, offset Password
    mov di, sp
    add di, 02h

    mov cx, 03h

llLoop:
    cmpsb
    jne llInccorect
    loop llLoop

    jmp llCorrect

llDone:

    add sp, STACK_ARRAY_LEN * 2 + 2 + 2

    ret

llInccorect:
    call PrintIncorrect

    jmp llDone

llCorrect:
    call PrintCorrect

    jmp llDone

endp

;---------------------------------


;---------------------------------
; Get the password
;
; Entry:  Input
; Exit:   Input
; Destrs: AH, BX, DX, CX, DI
;---------------------------------

GetPassword proc

    pop di si
    push si di

    mov ah, 0Ah

    mov cx, STACK_ARRAY_LEN

llFreeStackArray:
    push 0h
    loop llFreeStackArray

    push INPUT_MAX_LEN
    mov dx, sp

    int 21h

    inc dx
    mov bx, dx
    mov al, ss:[bx]
    inc dx
    mov ah, 0h
    add dx, ax
    mov bx, dx
    mov byte ptr ss:[bx], 0h

    mov bx, sp
    add bx, STACK_ARRAY_LEN * 2 + 2

    push ss:[bx]

    add bx, 02h
    mov ss:[bx], si

    ret

endp

;---------------------------------


;-------------DATA----------------

HelloMassage    db 'Hello, dear user!', 0Dh, 0Ah, 'Print the password or be caught:$'

CorrectResult   db 0Dh, 0Ah, 'YOU ARE A LIVING LEGEND$'

IncorrectResult db 0Dh, 0Ah, 'I GOT YOU$'

Password db 0h, 0h, 0h

;---------------------------------

end Main
