%define DRIVE 0x80
%define PLOC 0x9000
[bits 16]
[org 0]
start:
    mov ax,cs
    mov ds,ax
    mov es,ax
    mov ax,0x7000
    mov ss,ax
    mov sp,ss
    cmp byte [FIRST],0
    jnz skip_msg
    push WELCOME
    call print
    mov byte [FIRST],1
    skip_msg:
        mov ax,0x0200
        mov gs,ax
        mov bx,0
    show_prompt:
        push PROMPT
        call print
        push QUERY
        call read_cmd
        call search
        jmp show_prompt
; the function define
read_cmd:
    pusha
    mov bp,sp
    cld
    mov byte [CHARCOUNT],0
    mov di,[bp + 18]
    continue_read:
        mov ah,0
        int 16h
        cmp al,0dh
        jz fin
        mov ah,0x0e
        mov bx,0
        int 10h
        stosb
        inc byte [CHARCOUNT]
        jmp continue_read
    fin:
        push NL
        call print
        mov sp,bp
        popa
        ret
search:
    pusha
    mov bp,sp
    cmp ax,ax
    mov di,QUERY
    mov bx,0
    cld
    cont_chk:
        mov al,[gs:bx]
        cmp al,'}'
        je complete
        cmp al,[di]
        je chk
        inc bx
        jmp cont_chk
    chk:
        push bx
        mov cx,[CHARCOUNT]
    check:
        mov al,[gs:bx]
        inc bx
        scasb
        loope check
        je succ
        mov di,QUERY
        pop bx
        inc bx
        jmp cont_chk
    complete:
        push FAIL
        call print
        jmp en
    succ:
        inc bx
        push bx
        call find_sector
    en:
        mov sp,bp
        popa
        ret
find_sector:
    pusha
    mov bp,sp
    mov bx,[bp + 18]
    cld
    mov word [SECTOR],0
    mov cl,10
    count_sector:
        mov al,[gs:bx]
        inc bx
        cmp al,','
        jz finish
        cmp al,48
        jl mismatch
        cmp al,58
        jg mismatch
        sub al,48
        mov ah,0
        mov dx,ax
        mov ax,word [SECTOR]
        mul cl
        add ax,dx
        mov word [SECTOR],ax
        jmp count_sector
        finish:
            push word [SECTOR]
            call load
            jmp PLOC:0000
        mismatch_end:
            mov sp,bp
            popa
            ret
        mismatch:
            push FAIL
            call print
            jmp mismatch_end
load:
    pusha
    mov bp,sp
    mov ah,0
    mov dl,0x80
    int 0x13
    mov ax,PLOC
    mov es,ax
    mov cl,[bp + 18]
    mov al,1
    mov bx,0
    mov dl,DRIVE
    mov dh,0
    mov ch,0
    mov ah,2
    int 0x13
    jnc success
    err:
        push ERROR_LOAD
        call print
    success:
        mov sp,bp
        popa
        ret
print:
    pusha
    mov bp,sp
    mov si,[bp + 18]
    cont:
        lodsb
        or al,al
        jz dne
        mov ah,0x0e
        mov bx,0
        mov bl,7
        int 10h
        jmp cont
    dne:
        mov sp,bp
        popa
        ret
tohex:
    pusha
    mov bp,sp
    mov dx,[bp + 18]
    mov cx,4
    mov si,HEXCHAR
    mov di,HEX + 2
    stor:
        rol dx,4
        mov bx,15
        and bx,dx
        mov al,[si + bx]
        stosb
        loop stor
        push HEX
        call print
        mov sp,bp
        popa
        ret
WELCOME db "Carbon OS is booting finish!Type help or about to know more",10,13,0
ERROR_LOAD db "Error loading sector",10,13,0
FAIL db "File not found !",0
QUERY times 30 db 0
ARR times 10 db 0
FIRST_TIME db 1
NL db 10,13,0
PROMPT db 10,13, ">>",0
CHARCOUNT dw 0
HEX db "0x0000",10,13,0
HEXCHAR db "0123456789ABCDEF"
FIRST db 0
SECTOR dw 0
