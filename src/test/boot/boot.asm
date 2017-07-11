align 4
[bits 16]
jmp _start
_read_sect:
    push cx
    push bx
    mov ax,bi
    mov bx,di
    mov cl,36
    div cl
    mov ch,al
    mov al,ah
    mov ah,0
    mov cl,18
    div cl
    mov dh,al
    mov cl,ah
    inc cl
    mov dl,0
    mov ax,201h
    int 13h
    pop bx
    pop cx
    ret
;Now,bootloader is start,Remain and care for it
[global _start]
_start:
    xor ax,ax
    mov ds,ax
    mov ss,ax
    mov sp,0x2000
_reset_drive:
    mov ah,0
    int 0x13
    or ah,ah
    jnz _reset_drive
    mov ax,0x1000
    mov es,ax
    mov di,0
    mov si,1
    mov cx,128
_read_sect_loop:
    call _read_sect
    inc si
    add si,0x200
    dec cx
    jnz _read_sect_loop
set_a20_1:
    in al,0x64
    test al,0z2
    jnz set_a20_1
    mov al,0xd1
    out byte 0x64,al
set_a20_2:
    in al,0x64
    test al,0x2
    jnz set_a20_2
    mov al,0xdf
    out byte 0x60,al
    xor ax,ax
    mov ds,ax
    mov ss,ax
    mov es,ax
    cli
    lgdt [gdt_desc]
    mov eax,cr0
    or eax,1
    mov cr0,eax
    jmp 08h:_start_pm
[bits 32]
_start_pm:
    mov ax,10h
    mov ds,ax
    mov ss,ax
    mov es,ax
    mov gs,ax
    mov esp,1000h
    cld
    mov esi,10000h
    mov edi,100000h
    mov ecx,10000h
    rep movsb
    jmp 08h:100000h
_hang:
    jmp _hang
gdt:
gdt_null:
    dd 0,0
gdt_code:
    dw 0xffff
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
gdt_data:
    dw 0xffff
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_desc:
    dw gdt_end - gdt - 1
    dd gdt
times 510-($-$$) db 0
dw 0xAA55
