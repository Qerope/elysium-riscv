#include <ely/syscall.h>

#include <ely/platform.h>
#include <ely/sched.h>
long mach_msg_trap(struct trapframe *tf);

/* a7 = number, a0..a5 = arguments. Negative numbers are Mach traps, as in XNU. */
long syscall_dispatch(struct trapframe *tf)
{
    long num = (long)tf->x[17];
    if (num < 0) {
        switch (num) {
        case MACH_task_self_trap: return (long)current->task;
        case MACH_msg_trap: return mach_msg_trap(tf);
        }
        return -1;
    }
    switch (num) {
    case SYS_write: {
        const char *buf = (const char *)tf->x[11];
        for (long i = 0; i < (long)tf->x[12]; i++) kprintf("%c", buf[i]);
        return (long)tf->x[12];
    }
    case SYS_getpid: return current->id;
    case SYS_exit: current->state = T_DEAD; schedule(); return 0;
    }
    return -1;
}

long mach_msg_trap(struct trapframe *tf)
{
    /* a0 = port index, a1 = user message pointer, a2 = 1 to receive */
    return -1;
}
