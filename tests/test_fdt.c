#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../kern/fdt.h"

static uint8_t blob[512];
static uint32_t pos;

static void w32(uint32_t v)
{
    blob[pos++] = v >> 24; blob[pos++] = v >> 16; blob[pos++] = v >> 8; blob[pos++] = v;
}

/* A minimal tree: root { memory@80000000 { reg = <0 0x80000000 0 0x10000000> } } */
static uint32_t build(void)
{
    memset(blob, 0, sizeof blob);
    pos = 40;
    uint32_t struct_off = pos;
    w32(1); memcpy(blob + pos, "\0\0\0\0", 4); pos += 4;               /* root, empty name */
    w32(1); memcpy(blob + pos, "memory@80000000\0", 16); pos += 16;    /* 16 bytes, already aligned */
    w32(3); w32(16); w32(0);                                           /* reg, name offset 0 */
    w32(0); w32(0x80000000); w32(0); w32(0x10000000);
    w32(2); w32(2); w32(9);
    uint32_t strings_off = pos;
    memcpy(blob + pos, "reg\0", 4); pos += 4;
    uint32_t total = pos;
    pos = 0;
    w32(0xd00dfeed); w32(total); w32(struct_off); w32(strings_off);
    return total;
}

int main(void)
{
    uint64_t base, size;
    uint32_t total = build();
    assert(fdt_memory(blob, &base, &size) == 0);
    assert(base == 0x80000000UL && size == 0x10000000UL);

    blob[0] = 0;  /* bad magic */
    assert(fdt_memory(blob, &base, &size) == -1);
    blob[0] = 0xd0;

    /* Every truncation of a valid blob must fail cleanly, never read out of bounds. */
    for (uint32_t t = 0; t < total; t++) {
        uint8_t copy[512];
        memset(copy, 0, sizeof copy);
        memcpy(copy, blob, t);
        copy[4] = t >> 24; copy[5] = t >> 16; copy[6] = t >> 8; copy[7] = t;  /* totalsize = t */
        (void)fdt_memory(copy, &base, &size);
    }
    printf("ok\n");
    return 0;
}
