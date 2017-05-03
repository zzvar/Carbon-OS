;Protect partition test
;[Carbon OS team]
;write for c4droid
org 07c00h
jmp short boot
bootmsg db "Protect partition sector loading"
pmsg db "Loading finish,switch protect partition"
dw 512
db 1
dw 1
db 2
dw 0x00e0
dw 0x0b40
db 0x0f0
dw 9
dw 18
dw 2 ;读写扇区号
dw 0 ;隐藏扇区号
print:
    mov ah,0x13
    mov al,0x00
    mov bx,0x0007
    mov cx,0x20
    mov dx,0x0000
    int 0x10
    ret
get_key:
    mov ah,0x00
    int 0x16
    ret
clrscr:
    mov ax,0x0600
    mov cx,0x0000
    mov dx,0x174f
    mov bh,0
    int 0x10
    ret
boot:
    call clrscr
    mov bp,bootmsg
    call print
    call get_key
    bits 16
    call clrscr
    mov ax,0xb800
    mov gs,ax
    mov word [gs:0],0x641
    call get_key
    mov bp,pmsg
    call print
    call get_key
    call clrscr
    cli
    lgdt[gdtr]
    mov eax,cr0
    or al,0x01
    mov cr0,eax
    jmp codesel:pm
    bits 32
pm:
    mov ax,datasel
    mov ds,ax
    mov es,ax
    mov ax,videosel
    mov gs,ax
    mov word [gs:0],0x741
spin:
    jmp spin
bits 16
gdtr:
    dw gdt_end-gdt-1
    dd gdt
    gdt
    nullsel equ $-gdt
    gdt0
    dd 0
    dd 0
    codesel equ $-gdt
    code_gdt:
        dw 0x0ffff
        dw 0x0000
        db 0x00
        db 0x09a
        db 0x0cf
        db 0x00
        datasel equ $-gdt
    data_gdt:
        dw 0x0ffff
        dw 0x0000
        db 0x00
        db 0x092
        db 0x0cf
        db 0x00
        videosel equ $-gdt
        dw 3999
        dw 0x8000
        db 0x0b
        db 0x92
        db 0x00
        db 0x00
    gdt_end
times 510-($-$$) db 0
dw 0xaa55
