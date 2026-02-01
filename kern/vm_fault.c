#include <arch/pmap.h>
#include <ely/platform.h>
#include <ely/string.h>
#include <ely/task.h>
#include <arch/vm_page.h>

#define USER_HEAP_BASE 0x40000000UL
#define USER_HEAP_END  0x50000000UL

/* Page faults inside the heap region get a zeroed page on first touch. Returns 0 if handled. */
int vm_fault(struct task *t, uint64_t va)
{
    if (va < USER_HEAP_BASE || va >= USER_HEAP_END) return -1;
    uint64_t pa = vm_page_alloc();
    if (!pa) return -1;
    memset(phys_to_virt(pa), 0, PAGE_SIZE);
    return pmap_enter(&t->pmap, va & ~(PAGE_SIZE - 1), pa, PTE_R | PTE_W | PTE_U);
}
