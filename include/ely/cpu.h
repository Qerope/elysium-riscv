#pragma once
#include <ely/sched.h>

struct cpu {
    uint64_t hartid;
    struct thread *idle;
    volatile int online;
};

#define MAX_CPUS 8
extern struct cpu cpus[MAX_CPUS];

static inline struct cpu *this_cpu(void)
{
    struct cpu *c;
    __asm__ volatile("mv %0, tp" : "=r"(c));
    return c;
}

void smp_start_secondaries(uint64_t boot_hart, int nharts);
