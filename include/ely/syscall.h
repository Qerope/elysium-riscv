#pragma once
#include <arch/trap.h>

#define SYS_exit 1
#define SYS_write 4
#define SYS_getpid 20
#define MACH_task_self_trap (-28)
#define MACH_msg_trap (-31)
