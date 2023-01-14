#include <arch/sbi.h>
#include <csr.h>
#include <ely/platform.h>
#include <ely/sched.h>

#define TICK_HZ 100
#define TIMEBASE_HZ 10000000UL  /* qemu virt; later read from the device tree */

static uint64_t interval = TIMEBASE_HZ / TICK_HZ;

void timer_init(void)
{
    sbi_set_timer(csr_read(time) + interval);
}

void timer_intr(void)
{
    /* re-arm relative to the previous deadline to avoid drift */
    static uint64_t next;
    if (!next) next = csr_read(time);
    next += interval;
    sbi_set_timer(next);
}
