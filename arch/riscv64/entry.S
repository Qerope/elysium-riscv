    .section .text.entry
    .globl _start
_start:
    /* OpenSBI enters in S-mode: a0 = hartid, a1 = fdt (physical), MMU off */
    la sp, boot_stack_top
    call kmain
1:  wfi
    j 1b
    .section .bss
    .align 4
    .space 16384
    .globl boot_stack_top
boot_stack_top:
