[bits 16]
[org 0]
start:
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov ax,0xb800
	mov gs,ax
	mov bx,0
	mov si,MSG
	mov ch,1
	cont:
		lodsb 
		or al,al
		jz done
		mov byte [gs:bx],al
		mov byte [gs:bx + 1],ch
		inc ch
		add bx,2
	    jmp cont		
	done:
		jmp 0x1000:0
MSG db "Demonstration of Writing to Video memory",0
