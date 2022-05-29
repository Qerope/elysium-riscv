    .section .text.entry
    .globl _start
_start:
    /* OpenSBI enters in S-mode: a0 = hartid, a1 = fdt (physical), MMU off */
    la sp, boot_stack_top
    call boot_enable_paging      /* does not return: jumps to kernel_entry at its virtual address */
1:  wfi
    j 1b

    .globl kernel_entry
kernel_entry:
    la sp, boot_stack_top
    la t0, _bss
    la t1, _ebss
2:  bgeu t0, t1, 3f
    sd zero, 0(t0)
    addi t0, t0, 8
    j 2b
3:  call kmain
4:  wfi
    j 4b
    .section .bss
    .align 4
    .space 16384
    .globl boot_stack_top
boot_stack_top:
