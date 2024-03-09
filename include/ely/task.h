#pragma once
#include <arch/pmap.h>
#include <ely/sched.h>

struct task {
    pmap_t pmap;
    struct thread *threads;
    int id;
};

struct task *task_create(void);
