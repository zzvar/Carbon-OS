org 08200h 
jmp LABEL_BEGIN
DA_32	EQU	0x4000   ;D/BλΪ1,32λ��
DA_LIMIT_4K	EQU	8000h	; �ν������Ϊ 4K �ֽ�
DA_DPL0	EQU	0x00	;DPL=0;
DA_DPL1	EQU	0x20	;DPL=1;
DA_DPL2	EQU	0x40	;DPL=2;
DA_DPL3	EQU	0x60	;DPL=3;
; �洢�����������Pλ��Sλ��Ϊ�� TYPE Ϊ��Ӧ�����ֵ
DA_DR		EQU	90h	; ���ڵ�ֻ����ݶ�����ֵ��			
DA_DRW		EQU	92h	; ���ڵĿɶ�д��ݶ�����ֵ 
DA_DRWA		EQU	93h	; ���ڵ��ѷ��ʿɶ�д��ݶ�����ֵ
DA_C		EQU	98h	; ���ڵ�ִֻ�д��������ֵ
DA_CR		EQU	9Ah	; ���ڵĿ�ִ�пɶ���������ֵ
DA_CCO		EQU	9Ch	; ���ڵ�ִֻ��һ�´��������ֵ
DA_CCOR		EQU	9Eh	; ���ڵĿ�ִ�пɶ�һ�´��������ֵ
; ϵͳ�����������	PλΪ1	SλΪ0 ��TYPEΪ�����Ӧ��ֵ
DA_LDT		EQU	  82h	; �ֲ������������ֵ
DA_TaskGate	EQU	  85h	; ����������ֵ
DA_386TSS	EQU	  89h	; ���� 386 ����״̬������ֵ
DA_386CGate	EQU	  8Ch	; 386 ����������ֵ
DA_386IGate	EQU	  8Eh	; 386 �ж�������ֵ
DA_386TGate	EQU	  8Fh	; 386 ����������ֵ
;----------------------------------------------------------------------------
; ��ҳ����ʹ�õĳ��˵��
;----------------------------------------------------------------------------
PG_P		EQU	1	; ҳ��������λ
PG_RWR		EQU	0	; R/W ����λֵ, ��/ִ��
PG_RWW		EQU	2	; R/W ����λֵ, ��/д/ִ��
PG_USS		EQU	0	; U/S ����λֵ, ϵͳ��
PG_USU		EQU	4	; U/S ����λֵ, �û���
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
; ѡ��������ֵ˵��
; ����:
;       SA_  : Selector Attribute
SA_RPL0		EQU	0	; ��
SA_RPL1		EQU	1	; �� RPL
SA_RPL2		EQU	2	; ��
SA_RPL3		EQU	3	; ��
SA_TIG		EQU	0	; ��TI
SA_TIL		EQU	4	; ��
;�����ӵ͵��ߣ�
%macro Descriptor 3
	DW	%2 & 0xFFFF	;�ν��޵�ʮ��λ
	DW	%1 & 0xFFFF	;�εĻ��ַ��16λ
	DB	(%1>>16) & 0xFF	;�εĻ��ַ17-24λ
	DW	((%2>>8)&0x0F00)|(%3 & 0xF0FF) ;�εĽ��޸߰�λ������
	DB	(%1 >> 24) & 0xFF ;�εĻ��ַ��8λ
%endmacro
; ��
%macro Gate 4
	dw	(%2 & 0FFFFh)				; ƫ��1
	dw	%1					; ѡ����
	dw	(%3 & 1Fh) | ((%4 << 8) & 0FF00h)	; ����
	dw	((%2 >> 16) & 0FFFFh)			; ƫ��2
%endmacro ; �� 8 �ֽ�
;GDTȫ�ֱ����
[SECTION .gdt]
;�����
LABEL_GDT:		Descriptor	0,		0,			0				; �������
LABEL_DESC_VIDEO:	Descriptor	0B8000h,	0ffffh,			DA_DRW	+ DA_DPL3			; �Դ��׵�ַ
LABEL_DESC_STACK:	Descriptor	0,		TopOfStack,		DA_DRWA + DA_32			; Stack, 32 λ
LABEL_DESC_CODE32:	Descriptor	0,		SegCode32Len - 1,	DA_C + DA_32			; ��һ�´����
LABEL_DESC_CODE_DPL3:	Descriptor	0,		SegCodeDPL3Len - 1,	DA_C + DA_32+ DA_DPL3		; ��Ȩ��3�����
LABEL_DESC_DATA:	Descriptor	0,		DataLen-1,		DA_DRW	+ DA_DPL3		;�ɶ�д��ݶ�
LABEL_DESC_TESTYIZHI:	Descriptor	0,		TestYizhi-1,		DA_CCO + DA_32			;�����õ�һ�´���Σ�λ�ڸ���Ȩ

LABEL_DESC_CODE_DEST:  Descriptor 0, SegCodeDestLen-1, DA_C+DA_32; ��һ�´�������ڴ洢�Ŵ���

LABEL_DESC_STACK3:     Descriptor 0,      TopOfStack3, DA_DRWA+DA_32+DA_DPL3
LABEL_DESC_TSS:        Descriptor 0,          TSSLen-1, DA_386TSS

LABEL_CALL_GATE_TEST: Gate SelectorCodeDest,   0,     0, DA_386CGate+DA_DPL3 ;�Ŷ���

;�洢GDT��Ϣ
GdtLen		equ	$ - LABEL_GDT	; GDT����
GdtPtr		dw	GdtLen - 1	; GDT����
		dd	0		; GDT���ַ
; ��¼GDTѡ����
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT
SelectorCode32		equ	LABEL_DESC_CODE32	- LABEL_GDT
SelectorStack		equ	LABEL_DESC_STACK	- LABEL_GDT
SelectorData		equ	LABEL_DESC_DATA		- LABEL_GDT + SA_RPL3
SelectorDPL3		equ	LABEL_DESC_CODE_DPL3	- LABEL_GDT + SA_RPL3
SelectorStack3		equ	LABEL_DESC_STACK3	- LABEL_GDT + SA_RPL3
SelectorTSS		equ	LABEL_DESC_TSS		- LABEL_GDT
SelectorYIZHI		equ	LABEL_DESC_TESTYIZHI	- LABEL_GDT
SelectorCodeDest	equ	LABEL_DESC_CODE_DEST	- LABEL_GDT
SelectorCallGateTest	equ	LABEL_CALL_GATE_TEST	- LABEL_GDT + SA_RPL3

;GDT�������

; ȫ�ֶ�ջ�Ρ�����Ȩ��0
[SECTION .gs]
ALIGN	32
[BITS	32]
LABEL_DATA:
PMMESSAGE	:	db "In Protect Mode Now ...",0
PmOff		equ	PMMESSAGE - LABEL_DATA

helloMess:		db "Hello,NIHAO,Chine.......",0
heoff		equ	helloMess - LABEL_DATA

DataLen		equ	$-LABEL_DATA





; ȫ�ֶ�ջ�Ρ�����Ȩ��0
[SECTION .gs]
ALIGN	32
[BITS	32]
LABEL_STACK:
	times 512 db 0
TopOfStack	equ	$ - LABEL_STACK - 1
;ջ����















; ��ջ��ring3
[SECTION .s3]
ALIGN	32
[BITS	32]
LABEL_STACK3:
	times 512 db 0
TopOfStack3	equ	$ - LABEL_STACK3 - 1
; END of [SECTION .s3]




; TSS
[SECTION .tss]
ALIGN	32
[BITS	32]
LABEL_TSS:
		DD	0			; Back
		DD	TopOfStack		; 0 ����ջ
		DD	SelectorStack		; 
		DD	0			; 1 ����ջ
		DD	0			; 
		DD	0			; 2 ����ջ
		DD	0			; 
		DD	0			; CR3
		DD	0			; EIP
		DD	0			; EFLAGS
		DD	0			; EAX
		DD	0			; ECX
		DD	0			; EDX
		DD	0			; EBX
		DD	0			; ESP
		DD	0			; EBP
		DD	0			; ESI
		DD	0			; EDI
		DD	0			; ES
		DD	0			; CS
		DD	0			; SS
		DD	0			; DS
		DD	0			; FS
		DD	0			; GS
		DD	0			; LDT
		DW	0			; ���������־
		DW	$ - LABEL_TSS + 2	; I/Oλͼ��ַ
		DB	0ffh			; I/Oλͼ�����־
TSSLen		equ	$ - LABEL_TSS






















;16λ����Σ��������еĵط� 
[SECTION .s16]
[BITS	16]
LABEL_BEGIN:
    mov ax, cs
    mov ds, ax
    mov es, ax
 



	; ��ʼ�� 32 λ����������
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4			;������λ
	add	eax, LABEL_SEG_CODE32	;32λ�����ڴ��ַ��eax
	mov	word [LABEL_DESC_CODE32 + 2], ax ; ax�ĵ�ַ�������Ķλ��ַ	
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE32 + 4], al
	mov	byte [LABEL_DESC_CODE32 + 7], ah
	

	
	;��ʼ���ŵĴ����
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4			;������λ
	add	eax, LABEL_SEG_CODE_DEST	;32λ�����ڴ��ַ��eax
	mov	word [LABEL_DESC_CODE_DEST + 2], ax ; ax�ĵ�ַ�������Ķλ��ַ	
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE_DEST + 4], al
	mov	byte [LABEL_DESC_CODE_DEST + 7], ah


	
	; ��ʼ�����ò��Զ�
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4			;������λ
	add	eax, TestCodeHigh	;32λ�����ڴ��ַ��eax
	mov	word [LABEL_DESC_TESTYIZHI + 2], ax ; ax�ĵ�ַ�������Ķλ��ַ	
	shr	eax, 16
	mov	byte [LABEL_DESC_TESTYIZHI + 4], al
	mov	byte [LABEL_DESC_TESTYIZHI + 7], ah



	; ��ʼ����Ȩ��Ϊ3�Ĵ�����Զ�
	xor	eax, eax
	mov	ax, cs
	shl	eax, 4			;������λ
	add	eax, TESTCODE	;32λ�����ڴ��ַ��eax
	mov	word [LABEL_DESC_CODE_DPL3 + 2], ax ; ax�ĵ�ַ�������Ķλ��ַ	
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE_DPL3 + 4], al
	mov	byte [LABEL_DESC_CODE_DPL3 + 7], ah





	;��ʼ����ݶ�
	xor eax,eax
	mov ax,ds
	shl eax,4
	add eax,LABEL_DATA
	mov word[LABEL_DESC_DATA+2],ax
	shr eax,16
	mov byte[LABEL_DESC_DATA + 4], al
	mov byte[LABEL_DESC_DATA + 7], ah






	; ��ʼ����ջ�������
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_STACK
	mov	word [LABEL_DESC_STACK + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_STACK + 4], al
	mov	byte [LABEL_DESC_STACK + 7], ah


	; ��ʼ����ջ�������
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_STACK3
	mov	word [LABEL_DESC_STACK3 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_STACK3 + 4], al
	mov	byte [LABEL_DESC_STACK3 + 7], ah


	;	��ʼ��TTS��
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_TSS
	mov	word [LABEL_DESC_TSS + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_TSS + 4], al
	mov	byte [LABEL_DESC_TSS + 7], ah



	; Ϊ���� GDTR ��׼��
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_GDT		; eax <- gdt ���ַ
	mov	dword [GdtPtr + 2], eax	; [GdtPtr + 2] <- gdt ���ַ


	; Ϊ���� IDTR ��׼��
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_IDT		; eax <- idt ���ַ
	mov	dword [IdtPtr + 2], eax	; [IdtPtr + 2] <- idt ���ַ

	; ���� GDTR
	lgdt	[GdtPtr]

	; ���ж�
	cli

	; ���� IDTR
	lidt	[IdtPtr]
	
	
	; �򿪵�ַ��A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

	; ׼���л�������ģʽ
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	; ������뱣��ģʽ
	jmp	dword SelectorCode32:0	; ִ����һ���� SelectorCode32 װ�� cs,
;16λ����ν���
	
;32λ�����
[SECTION .s32]; 32 λ�����. ��ʵģʽ����.
[BITS	32]
LABEL_SEG_CODE32:
	mov	ax, SelectorStack
	mov	ss, ax			; ��ջ��ѡ����
	mov	esp, TopOfStack
	
	mov ax,SelectorVideo
	mov gs,ax
	mov ax,SelectorData
	mov ds,ax
	mov eax,1
	push eax
	mov eax,PmOff
	push eax
	call PrintString
	

	call Init8259A

	sti
	
	
	int 080h
	int 070h

	mov eax,2
	push eax
	mov eax,heoff
	push eax
	call PrintString
	
	mov	ax, SelectorTSS
	ltr	ax

	push	SelectorStack3
	push	TopOfStack3
	push	SelectorDPL3
	push	0
	retf

; Init8259A ---------------------------------------------------------------------------------------------
Init8259A:
	mov	al, 011h
	out	020h, al	; ��8259, ICW1.
	call	io_delay

	out	0A0h, al	; ��8259, ICW1.
	call	io_delay

	mov	al, 020h	; IRQ0 ��Ӧ�ж���� 0x20
	out	021h, al	; ��8259, ICW2.
	call	io_delay

	mov	al, 028h	; IRQ8 ��Ӧ�ж���� 0x28
	out	0A1h, al	; ��8259, ICW2.
	call	io_delay

	mov	al, 004h	; IR2 ��Ӧ��8259
	out	021h, al	; ��8259, ICW3.
	call	io_delay

	mov	al, 002h	; ��Ӧ��8259�� IR2
	out	0A1h, al	; ��8259, ICW3.
	call	io_delay

	mov	al, 001h
	out	021h, al	; ��8259, ICW4.
	call	io_delay

	out	0A1h, al	; ��8259, ICW4.
	call	io_delay

	mov	al, 11111110b	; �����ʱ���ж�
	;mov	al, 11111111b	; �����8259�����ж�
	out	021h, al	; ��8259, OCW1.
	call	io_delay

	mov	al, 11111111b	; ��δ�8259�����ж�
	out	0A1h, al	; ��8259, OCW1.
	call	io_delay

	ret
; Init8259A ---------------------------------------------------------------------------------------------

io_delay:
	nop
	nop
	nop
	nop
	ret
;--------------------------------------------------------------------------------------------

; int handler ---------------------------------------------------------------
_ClockHandler:
ClockHandler	equ	_ClockHandler - $$
	inc	byte [gs:((80 * 0 + 0) * 2)]	; ��Ļ�� 0 ��, �� 70 �С�
	mov	al, 20h
	out	20h, al				; ���� EOI
	iretd

_UserIntHandler:
UserIntHandler	equ	_UserIntHandler - $$
	mov	ah, 0Ch				; 0000: �ڵ�    1100: ����
	mov	al, 'I'
	mov	[gs:((80 * 13 + 0) * 2)], ax	; ��Ļ�� 0 ��, �� 70 �С�
	iretd

_SpuriousHandler:
SpuriousHandler	equ	_SpuriousHandler - $$
	mov	ah, 0Ch				; 0000: �ڵ�    1100: ����
	mov	al, '!'
	mov	[gs:((80 * 5 + 0) * 2)], ax	; ��Ļ�� 0 ��, �� 75 �С�
	iretd
; ---------------------------------------------------------------------------





PrintString:
	push ebp
	mov ebp,esp
	sub esp,8	
	
	mov eax,[ebp+12]
	;mov eax,1;��
	mov [ebp-4],eax
	mov eax,0;��
	mov [ebp-8],eax
	
	xor esi,esi
	mov esi,[ebp+8]
	.1:
	lodsb
	cmp al,0
	jz return
	push eax
	
	xor eax,eax
	mov eax,[ebp-4]
	mov bl,80
	mul bl
	add eax,[ebp-8]
	mov bl,2
	mul bl
	push eax
	
	call printfchar
	
	xor eax,eax
	mov eax,[ebp-8];��
	add eax,1
	mov [ebp-8],eax
	jmp .1
	return:
	mov esp,ebp
	pop ebp
	ret

printfchar:	
	push ebp
	mov ebp,esp
	mov edi,[ebp+8]
	mov al,byte[ebp+12]
	mov ah,0x0F
	mov [gs:edi], ax
	mov esp,ebp
	pop ebp
	ret
SegCode32Len	equ	$ - LABEL_SEG_CODE32
;32����ν���

;һ�´�����Զ�
[SECTION .CODETEST]
[BITS	32]
TestCodeHigh:
	mov edi,(80 *10 +2)*2
	mov al,'H'
	mov ah,0x0f
	mov [gs:edi],ax
	retf
TestYizhi equ $ - TestCodeHigh



;��Ȩ��3�Ĵ����
[SECTION .test]; 32 λ�����. ��ʵģʽ����.
[BITS	32]
TESTCODE:
	mov edi, (80 * 11 + 00) * 2
	mov al,'T'
	mov ah,0x0f

	mov [gs:edi],ax
	call SelectorYIZHI:0
	call	SelectorCallGateTest:0
	jmp $
SegCodeDPL3Len  equ $ - TESTCODE


[SECTION .sdest]; ������Ŀ���
[BITS	32]

LABEL_SEG_CODE_DEST:
	mov	ax, SelectorVideo
	mov	gs, ax			; ��Ƶ��ѡ����(Ŀ��)

	mov	edi, (80 * 12 + 0) * 2	; ��Ļ�� 12 ��, �� 0 �С�
	mov	ah, 0Ch			; 0000: �ڵ�    1100: ����
	mov	al, 'C'
	mov	[gs:edi], ax
	
	retf

SegCodeDestLen	equ	$ - LABEL_SEG_CODE_DEST
; END of [SECTION .sdest]









; IDT
[SECTION .idt]
ALIGN	32
[BITS	32]
LABEL_IDT:
; ��                        Ŀ��ѡ����,            ƫ��, DCount, ����
%rep 32
		Gate	SelectorCode32, SpuriousHandler,      0, DA_386IGate
%endrep
.020h:		Gate	SelectorCode32,    ClockHandler,      0, DA_386IGate
%rep 95
		Gate	SelectorCode32, SpuriousHandler,      0, DA_386IGate
%endrep
.080h:		Gate	SelectorCode32,  UserIntHandler,      0, DA_386IGate

IdtLen		equ	$ - LABEL_IDT
IdtPtr		dw	IdtLen - 1	; �ν���
		dd	0		; ���ַ
; END of [SECTION .idt]
