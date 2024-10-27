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
    boot_done = 1;
    __asm__ volatile("fence rw, rw" ::: "memory");
    for (int h = 0; h < nharts && h < MAX_CPUS; h++) {
        if ((uint64_t)h == boot_hart) continue;
        cpus[h].hartid = h;
        sbi_hart_start(h, (long)virt_to_phys(secondary_entry), (long)virt_to_phys(&cpus[h]));
    }
}

void secondary_main(struct cpu *c)
{
    while (!boot_done)
        __asm__ volatile("fence r, r" ::: "memory");
    __asm__ volatile("fence rw, rw" ::: "memory");
    c->online = 1;
    for (;;) __asm__ volatile("wfi");
}
