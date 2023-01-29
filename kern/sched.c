#include <ely/sched.h>

#include <ely/platform.h>

struct thread *current;
static struct thread *idle_thread;
static struct thread *head[NPRI], *tail[NPRI];
static uint32_t bitmap;

void sched_init(struct thread *idle)
{
    idle_thread = idle;
    idle->state = T_RUN;
    current = idle;
}

static void enqueue_locked(struct thread *t)
{
    t->state = T_READY;
    t->next = NULL;
    if (tail[t->pri]) tail[t->pri]->next = t;
    else head[t->pri] = t;
    tail[t->pri] = t;
    bitmap |= 1U << t->pri;
}

static struct thread *pick_locked(void)
{
    if (!bitmap) return idle_thread;
    int pri = 31 - __builtin_clz(bitmap);
    struct thread *t = head[pri];
    head[pri] = t->next;
    if (!head[pri]) { tail[pri] = NULL; bitmap &= ~(1U << pri); }
    return t;
}

void sched_enqueue(struct thread *t)
{
    enqueue_locked(t);
}

void schedule(void)
{
    struct thread *prev = current;
    if (prev->state == T_RUN && prev != idle_thread) enqueue_locked(prev);
    struct thread *next = pick_locked();
    next->state = T_RUN;
    if (next != prev) {
        current = next;
        ctx_switch(&prev->ctx, &next->ctx);
    }
}
