#include "trap.h"

#include <csr.h>
#include <ely/platform.h>

extern void trap_entry(void);

void trap_init(void)
{
    csr_write(stvec, trap_entry);
}

void trap_handler(struct trapframe *tf)
{
    uint64_t cause = tf->scause;
    int is_intr = cause >> 63;
    cause &= ~(1UL << 63);
    if (is_intr) {
        switch (cause) {
        default: kprintf("unexpected interrupt %d\n", (int)cause); break;
        }
        return;
    }
    kprintf("fault: scause=%d sepc=%p stval=%p\n", (int)cause, (void *)tf->sepc, (void *)tf->stval);
    panic("unhandled exception");
}
