# Design notes

## Boot

OpenSBI starts the kernel in S-mode at 0x80200000 with a0 = hartid and
a1 = physical address of the device tree. The image is linked at
0xffffffff80000000 + (load offset); `boot_enable_paging` builds a root table
with one gigapage mapping RAM at both the identity address and the kernel
address, turns on Sv39 and jumps to the virtual `kernel_entry`.
`kmain` removes the identity entry again.

Limitation: only the first gigabyte of RAM is mapped this way. A full
direct map is on the list.

## Objects

Mach style: a *task* owns an address space and a set of threads; *ports* are
the only way tasks talk to each other. System calls with a negative number are
Mach traps, positive numbers are the BSD layer.

## Porting to arm64

Real phone-class hardware is AArch64. The kernel is currently RISC-V only. A
port needs: an `arch/arm64` tree (entry, exception vectors, MMU with 4K
granule), a GICv3 driver instead of the PLIC, and the generic timer instead of
the SBI timer. Anything in `kern/` that includes `<csr.h>` directly is a bug.
