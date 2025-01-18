#include <ely/syscall.h>

#include <ely/platform.h>

/* a7 = number, a0..a5 = arguments. Negative numbers are Mach traps, as in XNU. */
long syscall_dispatch(struct trapframe *tf)
{
    long num = (long)tf->x[17];
    if (num < 0) {
        return -1;
    }
    return -1;
}
