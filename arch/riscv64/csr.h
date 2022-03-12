#pragma once
#include <stdint.h>

#define csr_read(csr) ({ uint64_t __v; __asm__ volatile("csrr %0, " #csr : "=r"(__v)); __v; })
#define csr_write(csr, v) __asm__ volatile("csrw " #csr ", %0" :: "r"((uint64_t)(v)))
#define csr_set(csr, v) __asm__ volatile("csrs " #csr ", %0" :: "r"((uint64_t)(v)))
#define csr_clear(csr, v) __asm__ volatile("csrc " #csr ", %0" :: "r"((uint64_t)(v)))

#define SSTATUS_SIE (1UL << 1)
#define SSTATUS_SPIE (1UL << 5)
#define SSTATUS_SPP (1UL << 8)
#define SIE_STIE (1UL << 5)
#define SIE_SEIE (1UL << 9)
#define SATP_SV39 (8UL << 60)

static inline void tlb_flush_va(uint64_t va) { __asm__ volatile("sfence.vma %0, zero" :: "r"(va) : "memory"); }
static inline void tlb_flush_all(void) { __asm__ volatile("sfence.vma zero, zero" ::: "memory"); }

static inline unsigned long irq_save(void) {
    unsigned long f;
    __asm__ volatile("csrrc %0, sstatus, %1" : "=r"(f) : "r"(SSTATUS_SIE) : "memory");
    return f & SSTATUS_SIE;
}
static inline void irq_restore(unsigned long f) {
    if (f) csr_set(sstatus, SSTATUS_SIE);
}
