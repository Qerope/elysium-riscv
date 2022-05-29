#include <stdint.h>

#include <csr.h>

#define KERNBASE 0xffffffff80000000UL
#define PHYS_BASE 0x80000000UL
#define PTE_V (1UL << 0)
#define PTE_R (1UL << 1)
#define PTE_W (1UL << 2)
#define PTE_X (1UL << 3)
#define PTE_G (1UL << 5)
#define PTE_A (1UL << 6)
#define PTE_D (1UL << 7)

/* In .data (not .bss): bss is cleared after paging is on, which would wipe the live tables. */
__attribute__((section(".data.boot"), aligned(4096))) uint64_t early_root[512];

extern char kernel_entry[];

/* Runs with the MMU off at the physical load address, but is linked at a high address.
   All symbol references here are pc-relative, so &early_root and &kernel_entry evaluate
   to *physical* addresses, which is exactly what the page table and satp need. */
void boot_enable_paging(uint64_t hartid, uint64_t dtb)
{
    uint64_t gigapage = ((PHYS_BASE >> 12) << 10) | PTE_V | PTE_R | PTE_W | PTE_X | PTE_G | PTE_A | PTE_D;
    uint64_t *root = early_root;

    root[(PHYS_BASE >> 30) & 0x1ff] = gigapage;  /* identity, needed for the next few instructions */
    root[(KERNBASE >> 30) & 0x1ff] = gigapage;   /* kernel at the top of the address space */

    uint64_t kentry = (uint64_t)kernel_entry - PHYS_BASE + KERNBASE;
    register uint64_t a0 __asm__("a0") = hartid;
    register uint64_t a1 __asm__("a1") = dtb;
    __asm__ volatile(
        "sfence.vma zero, zero\n"
        "csrw satp, %0\n"
        "sfence.vma zero, zero\n"
        "jr %1\n"
        :: "r"(SATP_SV39 | ((uint64_t)root >> 12)), "r"(kentry), "r"(a0), "r"(a1) : "memory");
    __builtin_unreachable();
}
