CROSS ?= riscv64-unknown-elf-
CC := $(CROSS)gcc
CFLAGS := -std=gnu11 -O2 -g -Wall -Wextra -ffreestanding -fno-pic -mcmodel=medany \
          -march=rv64imac -mabi=lp64 -mno-relax -Iinclude -Iarch/riscv64
LDFLAGS := -nostdlib -T linker.ld

SRCS := arch/riscv64/entry.S kern/kmain.c
SRCS += kern/kprintf.c
SRCS += kern/fdt.c
SRCS += kern/vm_page.c
SRCS += arch/riscv64/boot.c
SRCS += arch/riscv64/pmap.c
SRCS += arch/riscv64/trap.S arch/riscv64/trap.c
SRCS += dev/timer.c
SRCS += arch/riscv64/switch.S kern/sched.c
SRCS += dev/uart.c
SRCS += dev/plic.c
SRCS += kern/task.c
SRCS += kern/ipc.c
SRCS += kern/smp.c
SRCS += kern/syscall.c
SRCS += kern/user.c

build/elysium.elf: $(SRCS) linker.ld
	mkdir -p build
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

build/elysium.bin: build/elysium.elf
	$(CROSS)objcopy -O binary $< $@

run: build/elysium.bin
	qemu-system-riscv64 -machine virt -nographic -m 256M -bios default \
		-device loader,file=$<,addr=0x80200000

HOSTCC ?= cc
HOSTFLAGS := -std=gnu11 -O1 -g -Wall -Wextra -DELY_HOST -Iinclude -Itests
TESTS := build/test_fdt build/test_pmap build/test_sched build/test_ipc

build/test_fdt: tests/test_fdt.c kern/fdt.c tests/host.c
	mkdir -p build
	$(HOSTCC) $(HOSTFLAGS) -o $@ $^

build/test_pmap: tests/test_pmap.c arch/riscv64/pmap.c kern/vm_page.c tests/host.c
	mkdir -p build
	$(HOSTCC) $(HOSTFLAGS) -Iarch/riscv64 -o $@ $^

build/test_sched: tests/test_sched.c kern/sched.c tests/host.c
	$(HOSTCC) $(HOSTFLAGS) -Iarch/riscv64 -o $@ $^

build/test_ipc: tests/test_ipc.c kern/ipc.c kern/sched.c tests/host.c
	$(HOSTCC) $(HOSTFLAGS) -Iarch/riscv64 -o $@ $^

test: $(TESTS)
	for t in $(TESTS); do $$t || exit 1; done

clean:
	rm -rf build
