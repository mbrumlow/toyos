
C_SOURCES = $(wildcard *.c )
HEADERS = $(wildcard *.h )
OBJ = ${C_SOURCES:.c=.o}

all: stage1.bin

stage1.bin: stage1_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -T linker.ld  $^ --oformat binary

%.o: %.c ${HEADERS}
	gcc -m32 -ffreestanding -I . -c $< -o $@

%.o: %.asm
	nasm $< -f elf32 -o $@

clean: 
	rm -f *.o
	rm -f *.bin

