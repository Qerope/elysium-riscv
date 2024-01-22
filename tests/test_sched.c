#include <assert.h>
#include <stdio.h>

#include <ely/sched.h>

void ctx_switch(struct ctx *old, struct ctx *new_) { (void)old; (void)new_; }

static struct thread idle, a, b, c;

int main(void)
{
    sched_init(&idle);
    a.id = 1; a.pri = 10;
    b.id = 2; b.pri = 10;
    c.id = 3; c.pri = 20;
    sched_enqueue(&a);
    sched_enqueue(&b);
    sched_enqueue(&c);

    schedule();
    assert(current == &c);    /* highest priority first */
    c.state = T_DEAD;
    schedule();
    assert(current == &a);    /* then FIFO within a level */
    schedule();
    assert(current == &b);    /* round robin */
    schedule();
    assert(current == &a);

    /* quantum expiry only switches when another thread of >= priority is waiting */
    current->quantum = 1;
    sched_tick();
    assert(current == &b);

    /* sleeping and waking */
    int chan;
    thread_block(&chan);
    assert(b.state == T_SLEEP);
    thread_wakeup(&chan);
    assert(b.state == T_READY);
    printf("ok\n");
    return 0;
}
