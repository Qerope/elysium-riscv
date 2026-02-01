#include "trap.h"

#include <csr.h>
#include <ely/platform.h>
#include <ely/task.h>

extern void trap_entry(void);
void timer_intr(void);
void plic_intr(void);
long syscall_dispatch(struct trapframe *tf);
int vm_fault(struct task *t, uint64_t va);

void trap_init(void)
{
    csr_write(stvec, trap_entry);
    csr_set(sie, SIE_STIE);
    csr_set(sie, SIE_SEIE);
}

void trap_handler(struct trapframe *tf)
{
    uint64_t cause = tf->scause;
    int is_intr = cause >> 63;
    cause &= ~(1UL << 63);
    if (is_intr) {
        switch (cause) {
        case 5: timer_intr(); break;
        case 9: plic_intr(); break;
        default: kprintf("unexpected interrupt %d\n", (int)cause); break;
        }
        return;
    }
    if ((cause == 12 || cause == 13 || cause == 15) && current->task && vm_fault(current->task, tf->stval) == 0)
        return;  /* instruction/load/store page fault, handled */
    if (cause == 8) {  /* ecall from U-mode */
        tf->sepc += 4;
        tf->x[10] = syscall_dispatch(tf);
        return;
    }
    kprintf("fault: scause=%d sepc=%p stval=%p\n", (int)cause, (void *)tf->sepc, (void *)tf->stval);
    panic("unhandled exception");
}
