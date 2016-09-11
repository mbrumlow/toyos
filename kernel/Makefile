
all: kernel.bin

kernel.bin: kernel_entry.o kernel.o 
	ld -m elf_i386 -o kernel.bin -Ttext 0x1000 $^ --oformat binary

kernel.o: kernel.c
	gcc -m32 -ffreestanding -c $< -o $@

kernel_entry.o: kernel_entry.asm
	nasm $< -f elf -o $@


clean: 
	rm -f *.o *.bin

