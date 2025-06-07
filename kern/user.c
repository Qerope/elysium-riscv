#include <arch/pmap.h>
#include <ely/platform.h>
#include <ely/string.h>
#include <ely/task.h>
#include <arch/vm_page.h>

#define USER_TEXT 0x10000UL
#define USER_STACK_TOP 0x7ffff000UL

/* Copy a flat binary to USER_TEXT (R+X) and give the task one stack page (R+W). */
int user_load(struct task *t, const void *image, size_t len)
{
    for (size_t off = 0; off < len; off += PAGE_SIZE) {
        uint64_t pa = vm_page_alloc();
        if (!pa) return -1;
        size_t n = len - off < PAGE_SIZE ? len - off : PAGE_SIZE;
        memset(phys_to_virt(pa), 0, PAGE_SIZE);
        memcpy(phys_to_virt(pa), (const char *)image + off, n);
        if (pmap_enter(&t->pmap, USER_TEXT + off, pa, PTE_R | PTE_X | PTE_U)) return -1;
    }
    uint64_t sp = vm_page_alloc();
    if (!sp) return -1;
    memset(phys_to_virt(sp), 0, PAGE_SIZE);
    return pmap_enter(&t->pmap, USER_STACK_TOP - PAGE_SIZE, sp, PTE_R | PTE_W | PTE_U);
}

void user_enter(struct task *t)
{
    csr_write(satp, SATP_SV39 | (virt_to_phys(t->pmap.root) >> 12));
    tlb_flush_all();
    csr_clear(sstatus, SSTATUS_SPP);   /* return to U-mode */
    csr_set(sstatus, SSTATUS_SPIE);    /* with interrupts on */
    csr_write(sepc, USER_TEXT);
    __asm__ volatile("mv sp, %0\n sret" :: "r"(USER_STACK_TOP) : "memory");
    __builtin_unreachable();
}
