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
    mov bp,MSG
    mov ah,013h
    mov al,1
    mov bx,2
    mov cx,161
    int 0x10
    ret
MSG db "echo: read user input and print it at screen",10,13,"reg:dump regsiters",10,13,"vidmem: demonstrates writing to Video Memory",10,13,"exit:reboot",10,13,"about: the OS info"
