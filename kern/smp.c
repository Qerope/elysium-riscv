#include <ely/cpu.h>

#include <arch/sbi.h>
#include <ely/platform.h>

struct cpu cpus[MAX_CPUS];
extern void secondary_entry(void);
static volatile int boot_done;

void smp_start_secondaries(uint64_t boot_hart, int nharts)
{
    cpus[boot_hart].hartid = boot_hart;
    cpus[boot_hart].online = 1;
    for (int h = 0; h < nharts && h < MAX_CPUS; h++) {
        if ((uint64_t)h == boot_hart) continue;
        cpus[h].hartid = h;
        sbi_hart_start(h, (long)virt_to_phys(secondary_entry), (long)virt_to_phys(&cpus[h]));
    }
}

void secondary_main(struct cpu *c)
{
    c->online = 1;
    for (;;) __asm__ volatile("wfi");
}
