#include <stdint.h>

#include <arch/sbi.h>
#include <ely/platform.h>
#include "fdt.h"
#include <arch/vm_page.h>
extern char _kend[];

void kmain(uint64_t hartid, uint64_t dtb)
{
    (void)hartid;
    kprintf("Elysium booting on hart %d\n", (int)hartid);
    uint64_t base, size;
    if (fdt_memory((const void *)dtb, &base, &size))
        panic("no memory node in device tree");
    kprintf("ram: %p, %x MiB\n", (void *)base, (unsigned)(size >> 20));
    uint64_t free_start = (uint64_t)_kend;
    for (;;) __asm__ volatile("wfi");
}
