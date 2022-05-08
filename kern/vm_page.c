#include <arch/vm_page.h>

#include <ely/platform.h>

struct free_page {
    struct free_page *next;
};

static struct free_page *free_list;

void vm_page_init(uint64_t base, uint64_t size)
{
    base = (base + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    for (uint64_t pa = base; pa + PAGE_SIZE <= base + size; pa += PAGE_SIZE)
        vm_page_free(pa);
}

uint64_t vm_page_alloc(void)
{
    struct free_page *p = free_list;
    if (!p) return 0;
    free_list = p->next;
    return virt_to_phys(p);
}

void vm_page_free(uint64_t pa)
{
    struct free_page *p = phys_to_virt(pa);
    p->next = free_list;
    free_list = p;
}
