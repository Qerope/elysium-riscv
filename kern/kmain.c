#include <stdint.h>

#include <arch/sbi.h>
#include <ely/platform.h>

void kmain(uint64_t hartid, uint64_t dtb)
{
    (void)hartid;
    kprintf("Elysium booting on hart %d\n", (int)hartid);
    for (;;) __asm__ volatile("wfi");
}
