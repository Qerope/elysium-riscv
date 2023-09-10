#include <arch/vm_page.h>

#include <ely/platform.h>

struct free_page {
    uint64_t magic;
    struct free_page *next;
};

#define FREE_MAGIC 0xE1E51E5DEADF5EEDUL

static struct free_page *free_list;
static uint64_t nfree;

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
    nfree--;
    p->magic = 0;
    return virt_to_phys(p);
}

int vm_page_free(uint64_t pa)
{
    struct free_page *p = phys_to_virt(pa);
    if (p->magic == FREE_MAGIC) return -1;  /* double free */
    for (size_t i = 2; i < PAGE_SIZE / 8; i++)
        ((uint64_t *)p)[i] = 0xDEADDEADDEADDEADUL;  /* poison: a use after free shows up quickly */
    p->magic = FREE_MAGIC;
    p->next = free_list;
    free_list = p;
    nfree++;
    return 0;
}

uint64_t vm_page_count_free(void) { return nfree; }
