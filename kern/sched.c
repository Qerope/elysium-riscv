#include <ely/sched.h>

#include <ely/platform.h>

struct thread *current;

void schedule(void)
{
    struct thread *prev = current;
}
