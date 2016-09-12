
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h )
OBJ = ${C_SOURCES:.c=.o}

all: os-image

run: all
	qemu-system-x86_64 -serial stdio os-image 

os-image : boot/boot_sect.bin kernel.bin
	cat $^ > os-image
	truncate -s +512 os-image

kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
	ld -m elf_i386 -o wtf.bin -Ttext 0x1000 $^ 

boot/boot_sect.bin: kernel.bin
	chmod +x ./scripts/blocks.sh
	nasm boot/boot_sect.asm -f bin -dKERNEL_SECTORS=`./scripts/blocks.sh kernel.bin` -o $@

%.o: %.c ${HEADERS}
	gcc -m32 -ffreestanding -I . -c $< -o $@

%.o: %.asm
	nasm $< -f elf32 -o $@

clean: 
	rm -f *.bin *.dis *.o os-image
	rm -f kernel/*.o boot/*.bin drivers/*.o

