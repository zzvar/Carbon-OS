org 07c00h
KERNEL_ADDR equ 0x820
mov bx,LOAD
call print
mov ah,0x02
mov al,1
mov ch,0
mov dh,0
mov cl,2
mov dl,0
mov ax,KERNEL_ADDR
mov es,ax
mov bx,0
int 13h
jnc RD_SUCCESS
jmp RD_ERROR
loop:
    jmp loop
SUCCESS:
    mov bx,RD_SUCCESS
    call print
    jmp loop
ERROR:
    mov bx,RD_ERROR
    call print
    jmp loop
print:
    mov ah,0x0e
    mov bp,bx
    mov al,[bp]
    cmp al,0
    jz return
    int 10h
    add bx,1
    jmp print
return:
    ret
LOAD:
    db "loading...",13,10,0
RD_SUCCESS:
    db "Read disk successful",13,10,0
RD_ERROR:
    db "Read disk error",13,10,0
times 510-($-$$) db 0
dw 0x55aa
times 510 db 0
dw 0x55aa
