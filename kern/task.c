#include <ely/task.h>

static struct task tasks[16];
static int ntasks;

struct task *task_create(void)
{
    if (ntasks == 16) return 0;
    struct task *t = &tasks[ntasks];
    t->id = ntasks++;
    if (pmap_init(&t->pmap) != 0) { ntasks--; return 0; }
    return t;
}
