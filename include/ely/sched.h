#pragma once
#include <stdint.h>

struct ctx {
    uint64_t ra, sp, s[12];
};

enum { T_RUN, T_READY, T_SLEEP, T_DEAD };

#define NPRI 32
#define QUANTUM_TICKS 5

struct thread {
    struct ctx ctx;
    int id, pri, state, quantum;
    struct thread *next;
    void *kstack;
};

void ctx_switch(struct ctx *old, struct ctx *new_);
