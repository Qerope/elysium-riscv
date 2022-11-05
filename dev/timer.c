#include <arch/sbi.h>
#include <csr.h>
#include <ely/platform.h>

#define TICK_HZ 100
#define TIMEBASE_HZ 10000000UL  /* qemu virt; later read from the device tree */

static uint32_t interval = TIMEBASE_HZ / TICK_HZ;

void timer_init(void)
{
    sbi_set_timer(csr_read(time) + interval);
}

void timer_intr(void)
{
    sbi_set_timer(csr_read(time) + interval);
}
