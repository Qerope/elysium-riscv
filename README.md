# Elysium

An experimental operating system kernel for RISC-V with a Mach/BSD style
structure (tasks, threads, ports, a BSD syscall layer). Original code, not
derived from XNU.

Goal: a complete consumer mobile OS. Status: far from it.
## Running

    make run          # needs riscv64-unknown-elf-gcc and qemu-system-riscv64
    make test         # host unit tests, no cross toolchain needed

## Status (see docs/design.md)

- Boot: OpenSBI (S-mode) -> `_start` -> higher-half Sv39 -> `kmain`
- Memory: page allocator, Sv39 pmap (W^X enforced), no demand paging yet
- Scheduling: priority run queues, preemptive, per-cpu
- Drivers: ns16550 UART, PLIC, SBI timer
- IPC: ports with bounded queues
- Not started: filesystem, networking, anything graphical
