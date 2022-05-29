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
    const void *fdt = phys_to_virt(dtb);
    if (fdt_memory(fdt, &base, &size))
        panic("no memory node in device tree");
    kprintf("ram: %p, %x MiB\n", (void *)base, (unsigned)(size >> 20));
    uint64_t free_start = virt_to_phys(_kend);
    vm_page_init(free_start, base + size - free_start);
    for (;;) __asm__ volatile("wfi");
}
