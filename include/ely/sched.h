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
    const void *wchan;
    struct thread *next;
    void *kstack;
    struct task *task;
};

void ctx_switch(struct ctx *old, struct ctx *new_);
void sched_init(struct thread *idle);
void sched_enqueue(struct thread *t);
void sched_tick(void);
void schedule(void);
extern struct thread *current;
void thread_block(const void *wchan);
void thread_wakeup(const void *wchan);
