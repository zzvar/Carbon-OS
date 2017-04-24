org 0x7c00
jmp start
start:
    cyls equ 10          
    org 07c00h
    jmp entry
    db 0x90
    db "CarbonIPL"
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
    db "Carbon OS IPL"    
    db "FAT12"        
    resb 18
entry:
    mov ax,0          
    mov ss,ax
    mov sp,0x7c00 
    mov ds,ax
    mov ax,0x0820
    mov es,ax
    mov ch,0
    mov dh,0
    mov cl,2
readloop:
    mov si,0
retry:
    mov ah,0x02
    mov al,1
    mov bx,0
    mov dl,0x00
    int 0x13
    jnc next
    add si,1
    cmp si,5
    jae error
    mov ah,0x00
    mov dl,0x00   
    int 0x13
    jmp retry
next:
    mov ax,es                 
    add ax,0x0020
    mov es,ax          
    add cl,1           
    cmp cl,18            
    jbe readloop
    mov  cl,1
    add  dh,1
    cmp dh,2
    jb readloop
    mov dh,0
    add ch,1
    cmp ch,cyls
    jb readloop
    mov ax,cs
    mov ds,ax
    mov es,ax
    call PrintStr                
    jmp $
    mov [0x0ff0],ch        
    jmp 0xc200
fin:
    hlt
    jmp fin
error:
    mov  si,msg
putloop:
    mov al,[si]
    add si,1            
    cmp al,0
    je fin
    mov ah,0x0e            
    mov bx,15            
    int 0x10            
    jmp putloop
PrintStr:                            
    mov ax,BootPoem
    mov bp,ax
    mov cx,170
    mov ax,01301h
    mov bx,00009h
    mov dh,10
    mov dl,0
    int 10h
    ret
msg:
    db  0x0a, 0x0a       
    db  "load error"
    db   0x0a          
    db   0
BootPoem:   
    db "Hold fast to dreams"
    db 0x0a
    db "For if dreams die"
    db 0x0a
    db "Life is a broken-winged bird "
    db 0x0a
    db "That can never fly"
    db 0x0a
    db "Hold fast to dreams"
    db 0x0a
    db "For when dreams go"
    db 0x0a
    db "Life is a barren field"
    db 0x0a
    db "Frozen only with snow"
    db 0x0a
    times 510-($-$$) db 0
    dw 0xaa55
