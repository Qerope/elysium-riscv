#pragma once
#include <stddef.h>
#include <stdint.h>

/* Returns 0 and fills base/size from the first memory node, -1 on a bad blob. */
int fdt_memory(const void *fdt, uint64_t *base, uint64_t *size);
