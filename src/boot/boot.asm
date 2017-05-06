BEGINSECTION EQU 2
CYLINDER EQU 0
HEAD EQU 0
COUNTSECTION EQU 10
BEIGNSEGMENT EQU 0x0800
OFFICE EQU 0x0000
org 07c00h 
jmp LABEL_BEGIN
[SECTION .s16]
[BITS 16]
ClrSc:			
	mov ah,0x00
	mov al,0x03
	int 10h
    ret
Read:
	mov ax,BEIGNSEGMENT
	mov es,ax
	mov ah,0x02
	mov al,COUNTSECTION
	mov cl,BEGINSECTION
	mov ch,CYLINDER
	mov dh,HEAD
	mov dl,0
	mov bx,OFFICE
	int 13h
	ret
LABEL_BEGIN:
	mov ax, cs
	mov ds, ax
	mov es, ax
	call ClrSc
	call Read
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
            "|            Carbon OS            |",0x0d,0x0a,\
			"|        version 0.1 alpha        |",0x0d,0x0a,\
			"|          Carbon OS Team         |",0x0d,0x0a,\
	        "----------------------------------",0x0d,0x0a,0x0
times 510-4-($-$$) db 0
dw 0xaa55
