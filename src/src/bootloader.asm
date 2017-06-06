%define LOCATION 0x1000
%define FILE_TABLE 0x2000
%define DRIVE 0x80
%define OS_SECTOR 3
%define FILE_TABLE_SECTOR 2
[bits 16]
[org 0]
jmp 0x7c00:start
start:
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov al,03h
    mov ah,0
    int 10h
    mov si,BOOT_MSG
    call print
    mov ah,0
    int 16h
    mov ax,LOCATION
    mov es,ax
    ; the OS mount sector
    mov cl,OS_SECTOR
    ; the sector number
    mov al,2
    call loadsector
    jmp LOCATION:0000
done:
    jmp $
loadsector:
    mov bx,0
    mov dl,DRIVE
    mov dh,0
    mov ch,0
    mov ah,2
    int 0x13
    jc error
    ret
error:
    mov si,ERROR_MSG
    call print
    ret
print:
    mov bp,sp
    cont:
        lodsb
        or al,al
        jz fin
        mov ah,0x0e
        mov bx,0
        int 10h
        jmp cont
fin:
    mov sp,bp
    ret
BOOT_MSG db "Booting successful..",10,13,"Press Any key to continue",10,13,10,13,0
ERROR_MSG db "Error loading sector ",10,13,0
times 510-($-$$) db 0
dw 0xaa55
