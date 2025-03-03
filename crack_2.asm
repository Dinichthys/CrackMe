.model tiny
.186
locals ll
.code
org 100h

PRINT_FUNC  equ 09h
INT_NUMBER  equ 03h
FAKE_STRING equ 0197h

Main:
    mov ax, 0h
    mov es, ax
    mov bx, 21h*4

    cli                                 ; Start of changing

    mov ax, word ptr es:[bx]
    mov RealDOSOfs, ax
    mov ax, word ptr es:[bx+2]
    mov RealDOSSeg, ax                  ; Save the address of previous Time Controller
    mov es:[bx], offset DOSint
    mov es:[bx+2], cs                   ; Load the address of my Time Controller

    sti                                 ; End of changing

    mov ax, 3100h
    mov dx, offset EndLabel
    shr dx, 4
    inc dx
    int 21h                             ; Stop and stay resident

DOSint  proc

    cmp ah, PRINT_FUNC
    je llCheckNumber

llContinue:
    db 0eah                             ; Jump to real DOS int Controller
RealDOSOfs dw 0h
RealDOSSeg dw 0h

llCheckNumber:
    inc cs:[Count]

    cmp cs:[Count], INT_NUMBER
    je llCrack

    jmp llContinue

llCrack:

    push ax bx es

    mov ax, 0h
    mov es, ax
    mov bx, 21h*4

    mov ax, cs:[RealDOSOfs]
    mov es:[bx], ax
    mov ax, cs:[RealDOSSeg]
    mov es:[bx+2], ax

    pop es bx ax

    mov dx, FAKE_STRING
    jmp llContinue

endp

Count db 0h

EndLabel:

end Main
