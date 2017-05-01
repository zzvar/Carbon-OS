a.img:boot.asm test.asm
	nasm boot.asm -o boot.bin 
	nasm test.asm -o test.bin 
	dd of=carbon.img if=boot.bin conv=notrunc
	dd of=carbon.img if=test.bin bs=512 seek=1 conv=notrunc
