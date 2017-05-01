org 0x8000
mov ax,cs
mov es,ax
mov ds,ax
mov ax,0xb800
mov ds,ax
mov di,0x100
mov ah,0x0c
mov al,'S'
mov [ds:di],ax
jmp $

