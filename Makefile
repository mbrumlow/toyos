
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h )
OBJ = ${C_SOURCES:.c=.o}

all: os-image

run: all
	qemu-system-i386 -m size=6411K -serial stdio -cpu 486 os-image 

os-image : boot/boot_sect.bin stage1/stage1.bin kernel/kernel.bin
	cat $^ > os-image
	truncate -s +512 os-image

stage1/stage1.bin: 
	make -C stage1 
	chmod +x ./scripts/pad.sh 
	./scripts/pad.sh $@

kernel/kernel.bin:
	make -C kernel 
	chmod +x ./scripts/pad.sh 
	./scripts/pad.sh $@

boot/boot_sect.bin: stage1/stage1.bin kernel/kernel.bin
	chmod +x ./scripts/blocks.sh
	nasm boot/boot_sect.asm -f bin -dSTAGE1_SECTORS=`./scripts/blocks.sh stage1/stage1.bin` -dKERNEL_SECTORS=`./scripts/blocks.sh kernel/kernel.bin` -o $@

clean: 
	rm -f *.bin os-image
	rm -f boot/boot_sect.bin
	make -C stage1 clean
	make -C kernel clean

