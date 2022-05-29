#pragma once
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096UL
#ifndef ELY_HOST
#define KERNBASE 0xffffffff80000000UL
#define PHYS_BASE 0x80000000UL
#define phys_to_virt(pa) ((void *)((uintptr_t)(pa) - PHYS_BASE + KERNBASE))
#define virt_to_phys(va) ((uint64_t)(va) - KERNBASE + PHYS_BASE)
#define mmio(pa) ((volatile void *)(0xffffffff40000000UL + (pa)))
#include <csr.h>
#else
void *phys_to_virt(uint64_t pa);
uint64_t virt_to_phys(const void *va);
#define tlb_flush_va(va) ((void)(va))
#define tlb_flush_all() ((void)0)
static inline unsigned long irq_save(void) { return 0; }
static inline void irq_restore(unsigned long f) { (void)f; }
#endif

void kprintf(const char *fmt, ...);
void panic(const char *fmt, ...) __attribute__((noreturn));
