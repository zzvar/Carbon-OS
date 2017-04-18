#create on 2017/3/20
#by wangcheng11
#[Carbon_OS team]

CC     = gcc
CFLAGS = -m32 -Wall -fno-stack-protector -Iinclude

OBJS   = boot.o \
	kernel.o \
	fastscreen.o \
	string/format.o \
	string/string.o \
	memory/memory.o \
	klog/klog.o \
	list/LinkedList.o \


all: boot.o kernel
boot.o:
	nasm -f elf boot.asm -o boot.o
	
kernel:$(OBJS)
	ld -T link.ld -m elf_i386 -nostdlib $(OBJS) -o kernel
	
clean:
	rm -f $(OBJS)
	rm -f *.o
	rm -f kernel