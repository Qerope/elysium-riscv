#pragma once
#include <stddef.h>
#include <stdint.h>

/* Returns 0 and fills base/size from the first memory node, -1 on a bad blob. */
int fdt_memory(const void *fdt, uint64_t *base, uint64_t *size);
/* Finds the first node whose "compatible" contains `compat`; fills the first reg entry. */
int fdt_find_reg(const void *fdt, const char *compat, uint64_t *base, uint64_t *size);
