#pragma once
#include <stdint.h>

struct trapframe {
    uint64_t x[32];
    uint64_t sepc, sstatus, scause;
};

void trap_init(void);
void trap_handler(struct trapframe *tf);
