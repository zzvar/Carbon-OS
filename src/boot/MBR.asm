SECTION ALIGN=16 VSTART=0x7c00
LBA_BASE_ADDRESS equ 5
mov ax,[cs:SS_BASE]
mov ss,ax
xor sp,sp
mov ax,[cs:DS_BASE]
mov ds,ax
xor bx,bx
mov cx,512
write_memory:
    mov byte [bx],0x02
    inc bx
    loop write_memory
    xor bx,bx
    call write_disk
    jmp $
write_disk:
    mov dx,0x1f2
    mov al,0x01
    call out_port
    call pio_delay
    mov dx,0x1f3
    mov al,0x03
    call out_port
    call pio_delay
    mov dx,0x1f4
    mov al,0x00
    call out_port
    call pio_delay
    mov dx,0x1f5
    mov al,0x00
    call out_port
    call pio_delay
    mov dx,0x1f6
    mov al,0xe0
    call out_port
    call pio_delay
    call check_drdy
busy_hd:
    call read_status
    and al,0x80
    jnz busy_hd
accept_hd:
    call read_status
    and al,0x40
    jz accept_hd
    mov dx,0x1f7
    mov al,0x30
    call out_port
    call pio_delay
busy_hd1:
    call read_status
    and al,0x80
    jnz busy_hd1
accept_hd2:
    call read_status
    and al,0x88
    cmp al,0x08
    jnz accept_hd2
    mov cx,512
    mov al,0x12
    mov dx,0x1f0
s:
    call out_port
    loop s
    mov dx,0x1f0
    mov di,bx
    mov cx,256
    cld
    rep outsw
accept_hd3:
    call read_status
    test al,0x40
    jz accept_hd3
    ret
out_port:
    out dx,al
    ret
check_drdy:
    mov dx,0x1f7
    in al,dx
    test al,0x40
    jz check_drdy
    ret
read_status:
    mov dx,0x1f7
    in al,dx
    ret
pio_delay:
    nop
    nop
    nop
    nop
    ret
DS_BASE dw 0x1000
SS_BASE dw 0x2000
TARGET_PROGRAM_ADDRESS db 0x02
TARGET_PROGRAM_SIZE db 0x01
times 510-($-$$) db 0
dw 0xaa55
