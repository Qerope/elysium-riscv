CROSS ?= riscv64-unknown-elf-
CC := $(CROSS)gcc
CFLAGS := -std=gnu11 -O2 -g -Wall -Wextra -ffreestanding -fno-pic -mcmodel=medany \
          -march=rv64imac -mabi=lp64 -mno-relax -Iinclude -Iarch/riscv64
LDFLAGS := -nostdlib -T linker.ld

SRCS := arch/riscv64/entry.S kern/kmain.c

build/elysium.elf: $(SRCS) linker.ld
	mkdir -p build
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

build/elysium.bin: build/elysium.elf
	$(CROSS)objcopy -O binary $< $@

run: build/elysium.bin
	qemu-system-riscv64 -machine virt -nographic -m 256M -bios default \
		-device loader,file=$<,addr=0x80200000
