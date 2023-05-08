#include <stdint.h>

#include <arch/sbi.h>
#include <ely/platform.h>
#include "fdt.h"
#include <arch/vm_page.h>
#include <arch/trap.h>
#include <ely/sched.h>

void uart_init(const void *fdt);
void timer_init(void);
void boot_drop_identity(void);
extern char _kend[];

static struct thread idle;

void kmain(uint64_t hartid, uint64_t dtb)
{
    (void)hartid;
    kprintf("Elysium booting on hart %d\n", (int)hartid);
    boot_drop_identity();
    uint64_t base, size;
    const void *fdt = phys_to_virt(dtb);
    if (fdt_memory(fdt, &base, &size))
        panic("no memory node in device tree");
    kprintf("ram: %p, %x MiB\n", (void *)base, (unsigned)(size >> 20));
    uint64_t free_start = virt_to_phys(_kend);
    vm_page_init(free_start, base + size - free_start);
    uart_init(fdt);
    trap_init();
    sched_init(&idle);
    timer_init();
    csr_set(sstatus, SSTATUS_SIE);
    for (;;) __asm__ volatile("wfi");
}
