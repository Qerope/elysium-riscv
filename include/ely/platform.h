#pragma once
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096UL
#define phys_to_virt(pa) ((void *)(uintptr_t)(pa))
#define virt_to_phys(va) ((uint64_t)(uintptr_t)(va))

void kprintf(const char *fmt, ...);
void panic(const char *fmt, ...) __attribute__((noreturn));
