a.img:boot/boot.asm boot/test.asm
	nasm boot/boot.asm -o bin/boot.bin 
	nasm boot/test.asm -o bin/test.bin 
	dd of=out/carbon.img if=bin/boot.bin conv=notrunc
	dd of=out/carbon.img if=bin/test.bin bs=512 seek=1 conv=notrunc
