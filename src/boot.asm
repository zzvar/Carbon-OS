;定义开始扇区的位置
BEGINSECTION  	EQU		2
;定义柱面
CYLINDER    EQU     0
;定义磁道
HEAD       EQU     0
;定义要读取的扇区数
COUNTSECTION       EQU     10
;读取到的位置
BEIGNSEGMENT		EQU		0x0800
OFFICE		EQU		0x0000


;以下定义ipl的内容
org 07c00h 
jmp LABEL_BEGIN
[SECTION .s16]
[BITS	16]
;---------------------------------
;清理屏幕
ClrSc:			
	mov ah,0x00
	mov al,0x03
	int 10h
    ret
;-------------------------------------
;读取扇区内容
Read:
	mov ax,BEIGNSEGMENT
	mov es,ax
	mov ah,0x02 ;功能号
	mov al,COUNTSECTION   ;扇区数
	mov cl,BEGINSECTION	;扇区
	mov ch,CYLINDER	;柱面
	mov dh,HEAD	;磁头
	mov dl,0	;驱动器
	mov bx,OFFICE	
	int 13h
	ret
;---------------------------
;开始运行的位置
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

	;jmp	BEIGNSEGMENT:OFFICE
msg db 0x0a,"----------------------------------",0x0d,0x0a,\
            "|            Carbon OS           |",0x0d,0x0a,\
			"|        version 0.1 alpha       |",0x0d,0x0a,\
			"|          Carbon OS Team        |",0x0d,0x0a,\
	        "----------------------------------",0x0d,0x0a,0x0
times 510-4-($-$$) db 0
dw 0xaa55
