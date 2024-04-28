#include <ely/ipc.h>

#include <ely/platform.h>
#include <ely/sched.h>
#include <ely/string.h>

/* Non-blocking; a full queue is the sender's problem, not ours to buffer. */
int port_send(struct port *p, const struct msg *m)
{
    unsigned long f = irq_save();
    if (p->count == PORT_QUEUE) { irq_restore(f); return -1; }
    p->q[(p->head + p->count) % PORT_QUEUE] = *m;
    p->count++;
    irq_restore(f);
    thread_wakeup(p);
    return 0;
}

int port_receive(struct port *p, struct msg *out)
{
    while (p->count == 0)
        thread_block(p);
    *out = p->q[p->head];
    p->head = (p->head + 1) % PORT_QUEUE;
    p->count--;
    return 0;
}
