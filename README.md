# Elysium

An experimental operating system kernel for RISC-V with a Mach/BSD style
structure (tasks, threads, ports, a BSD syscall layer). Original code, not
derived from XNU.

Goal: a complete consumer mobile OS. Status: far from it.
## Running

    make run          # needs riscv64-unknown-elf-gcc and qemu-system-riscv64
    make test         # host unit tests, no cross toolchain needed
