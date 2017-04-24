;fat12 boot sector
org 0x7c00
jmp _START
times 3-($-$$) nop
db "Carbon OS Team"
dw 512
db 1
dw 1
db 2
dw 224
dw 2880
db 0xf0
dw 9
dw 18
dw 2
dd 0
dd 2880
db 0,0,0x29
dd 0xffffffff
db "Carbon OS v0.1 amd64"
db "File System:FAT12"
times 18 db 0
_START:
    mov ax,0
	mov si,msg
_LOOP:
    mov al,[si]
	add si,1
	cmp al,0
	je _END
	mov ah,0x0e
	mov bx,15
	int 0x10
	jmp _LOOP
_END:
    jmp $
msg db 0x0a,"----------------------------------",0x0d,0x0a,\
         "|            Carbon OS           |",0x0d,0x0a,\
		 "----------------------------------",0x0d,0x0a,0x0
    times 510-($-$$) db 0
	dw 0xaa55
	db 0xf0,0xff,0xff,0x00,0x00,0x00,0x00
	times 4600 db 0
	db 0xf0,0xff,0xff,0x00,0x00,0x00,0x00
	times 1469432 db 0
