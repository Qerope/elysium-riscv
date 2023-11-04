# ADR 0001: tasks, threads and ports

Status: accepted

Context: we want process isolation and a small kernel with servers in user
space eventually (file systems, networking, graphics).

Decision: tasks and threads are separate objects; communication goes through
ports with bounded queues. The queue is bounded so a misbehaving sender cannot
make the kernel allocate without limit. Messages are copied (64 bytes inline
for now); out-of-line data comes later.

Consequences: every blocking receive needs a wakeup that cannot be lost. The
check-and-block sequence therefore runs with interrupts disabled (see the fix
in ipc.c).
