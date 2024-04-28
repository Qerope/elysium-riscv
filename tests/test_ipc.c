#include <assert.h>
#include <stdio.h>

#include <ely/ipc.h>
#include <ely/sched.h>

void ctx_switch(struct ctx *old, struct ctx *new_) { (void)old; (void)new_; }

int main(void)
{
    static struct port p;
    struct msg m = {.id = 7, .len = 0}, out;

    assert(port_send(&p, &m) == 0);
    assert(port_receive(&p, &out) == 0);
    assert(out.id == 7);

    for (int i = 0; i < PORT_QUEUE; i++) {
        m.id = i;
        assert(port_send(&p, &m) == 0);
    }
    assert(port_send(&p, &m) == -1);        /* bounded: sender sees the error */
    for (int i = 0; i < PORT_QUEUE; i++) {
        assert(port_receive(&p, &out) == 0);
        assert(out.id == (uint32_t)i);      /* FIFO */
    }
    printf("ok\n");
    return 0;
}
