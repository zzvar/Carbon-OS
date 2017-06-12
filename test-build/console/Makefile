dev = carbon.img
ASMFLAGS = -f bin


all:bootloader os  about vidmem echo help reboot reg install
	dd if=/dev/zero of=carbon.img count=1
	./cpy $(dev) bootloader os about vidmem echo help reboot reg
	@echo "Done...Written to disk"
	

bootloader:bootloader.asm
	nasm $(ASMFLAGS) bootloader.asm

os:os.asm
	nasm $(ASMFLAGS) os.asm


reg:reg.asm
	nasm $(ASMFLAGS) reg.asm


about:about.asm
	nasm $(ASMFLAGS) about.asm

help:help.asm
	nasm $(ASMFLAGS) help.asm


vidmem:vidmem.asm
	nasm $(ASMFLAGS) vidmem.asm

echo:echo.asm
	nasm $(ASMFLAGS) echo.asm

reboot:reboot.asm
	nasm $(ASMFLAGS) reboot.asm


install:cpy.c
	cc cpy.c -o cpy
clean:
	rm bootloader
	rm os
	rm echo
	rm vidmem
	rm about
	rm -rf filetable
	rm help
	rm reg
	rm reboot
	rm carbon.img
	rm cpy
