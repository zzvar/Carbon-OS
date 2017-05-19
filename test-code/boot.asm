bits 16
org 0x7c00
jmp main
Message db "Hello World,Booting from low-level 16-bit",0x0
MessageB db "This bootloader is write of x86 assembly language",0x0
AnyKey db "Press any key to reboot",0x0
Println:
    lodsb
    or al,al
    jz complete
    mov ah,0x0e
    int 0x10
    jmp Println
complete:
    call PrintNwl
PrintNwl:
    mov al,0
    stosb
    mov ah,0x0e
    mov al,0x0d
    int 0x10
    mov al,0x0a
    int 0x10
    ret
Reboot:
    mov si,AnyKey
    call Println
    call GetPressedKey
    db 0x0ea
    dw 0x0000
    dw 0xffff
    mov ah,0
    int 0x16
    ret
GetPressedKey:
    mov ah,0
    int 0x16
    ret
main:
    cli
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov ss,ax
    sti
    mov si,Message
    call Println
    mov si,MessageB
    call Println
    call PrintNwl
    call PrintNwl
    call Reboot
times 510-($-$$) db 0
dw 0xaa55
