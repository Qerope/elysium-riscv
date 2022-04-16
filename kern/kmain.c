#include <stdint.h>

#include <arch/sbi.h>
#include <ely/platform.h>
#include "fdt.h"

void kmain(uint64_t hartid, uint64_t dtb)
{
    (void)hartid;
    kprintf("Elysium booting on hart %d\n", (int)hartid);
    uint64_t base, size;
    if (fdt_memory((const void *)dtb, &base, &size))
        panic("no memory node in device tree");
    kprintf("ram: %p, %x MiB\n", (void *)base, (unsigned)(size >> 20));
    for (;;) __asm__ volatile("wfi");
}
