CC := riscv64-unknown-elf-gcc
AS := riscv64-unknown-elf-as
LD := riscv64-unknown-elf-ld
QEMU := qemu-system-riscv32

FILES_C := main.c proga.c progb.c sbi.c io.c
FILES_O := boot.o sbi.o io.o main.o proga.o progb.o
X := bpO5
ARCH := rv32g

all: link

compile:  sbi.c
	$(CC) -std=c99 -march=$(ARCH) -mabi=ilp32 -c -o sbi.o sbi.c
	$(CC) -std=c99 -march=$(ARCH) -mabi=ilp32 -c -o io.o io.c
	$(CC) -std=c99 -march=$(ARCH) -mabi=ilp32 -c -o main.o main.c
	$(CC) -std=c99 -march=$(ARCH) -mabi=ilp32 -c -o proga.o proga.c
	$(CC) -std=c99 -march=$(ARCH) -mabi=ilp32 -c -o progb.o progb.c


assemble: boot.s
	$(AS) -march=$(ARCH) -mabi=ilp32 -o boot.o boot.s

link: compile assemble
	$(LD) -T boot.ld -m elf32lriscv ${FILES_O} -o $(X)
 
run: link
	$(QEMU) -machine virt -nographic -kernel $(X) -bios fw_dynamic32.bin

clean:
	rm $(X)
	rm $(FILES_O)

opensbi:
	git clone https://github.com/riscv-software-src/opensbi.git
	cd opensbi; \ 
	make CROSS_COMPILE=riscv64-linux-gnu- ARCH=riscv PLATFORM_RISCV_XLEN=>
	cp build/platform/generic/firmware/fw_dynamic.bin ../fw_dynamic32.bin;
