    .macro SAVE
    addi sp, sp, -280
    .irp n,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\n, \n*8(sp)
    .endr
    addi t0, sp, 280
    sd t0, 16(sp)
    csrr t0, sepc
    sd t0, 256(sp)
    csrr t0, sstatus
    sd t0, 264(sp)
    csrr t0, scause
    sd t0, 272(sp)
    .endm

    .section .text
    .align 4
    .globl trap_entry
trap_entry:
    SAVE
    mv a0, sp
    call trap_handler
    ld t0, 256(sp)
    csrw sepc, t0
    ld t0, 264(sp)
    csrw sstatus, t0
    .irp n,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\n, \n*8(sp)
    .endr
    addi sp, sp, 280
    sret
