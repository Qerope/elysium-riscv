#include "pmap.h"

#include <ely/platform.h>
#include <ely/string.h>

#include "vm_page.h"

#define PTE_A (1UL << 6)
#define PTE_D (1UL << 7)
#define VPN(va, lvl) (((va) >> (12 + 9 * (lvl))) & 0x1ff)

static inline uint64_t pte_pa(uint64_t pte) { return (pte >> 10) << 12; }
static inline uint64_t pa_pte(uint64_t pa) { return (pa >> 12) << 10; }

int pmap_init(pmap_t *pm)
{
    uint64_t pa = vm_page_alloc();
    if (!pa) return -1;
    pm->root = phys_to_virt(pa);
    memset(pm->root, 0, PAGE_SIZE);
    return 0;
}

/* Returns the leaf PTE slot for va, creating intermediate tables if asked. */
static uint64_t *walk(pmap_t *pm, uint64_t va, int create)
{
    uint64_t *pt = pm->root;
    for (int lvl = 2; lvl > 0; lvl--) {
        uint64_t *pte = &pt[VPN(va, lvl)];
        if (*pte & PTE_V) {
            if (*pte & (PTE_R | PTE_W | PTE_X)) return NULL;  /* a superpage is in the way */
            pt = phys_to_virt(pte_pa(*pte));
        } else {
            if (!create) return NULL;
            uint64_t pa = vm_page_alloc();
            if (!pa) return NULL;
            memset(phys_to_virt(pa), 0, PAGE_SIZE);
            *pte = pa_pte(pa) | PTE_V;
            pt = phys_to_virt(pa);
        }
    }
    return &pt[VPN(va, 0)];
}

int pmap_enter(pmap_t *pm, uint64_t va, uint64_t pa, uint64_t prot)
{
    uint64_t *pte = walk(pm, va, 1);
    if (!pte) return -1;
    if (*pte & PTE_V) return -1;  /* callers must pmap_remove first */
    *pte = pa_pte(pa) | prot | PTE_V | PTE_A | PTE_D;
    return 0;
}

int pmap_remove(pmap_t *pm, uint64_t va)
{
    uint64_t *pte = walk(pm, va, 0);
    if (!pte || !(*pte & PTE_V)) return -1;
    *pte = 0;
    return 0;
}

uint64_t pmap_extract(pmap_t *pm, uint64_t va)
{
    uint64_t *pte = walk(pm, va, 0);
    if (!pte || !(*pte & PTE_V)) return 0;
    return pte_pa(*pte) | (va & (PAGE_SIZE - 1));
}
