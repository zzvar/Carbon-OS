[bits 16]
[org 0]
start:
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov ah,03
    mov bh,0
    int 10h
    call printstr
    jmp 0x1000:0
printstr:
    mov bp,HELP_MSG
    mov ah,013h
    mov al,1
    mov bx,2
    mov cx,104
    int 0x10
    ret
HELP_MSG db "Maker:Anonymous",10,13,"Contect:c4droid@foxmail.com",10,13,"Work at Ubuntu 16.04"
