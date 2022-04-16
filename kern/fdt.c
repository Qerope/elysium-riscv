#include "fdt.h"

#include <ely/string.h>

#define FDT_MAGIC 0xd00dfeedU
#define FDT_BEGIN_NODE 1
#define FDT_END_NODE 2
#define FDT_PROP 3
#define FDT_NOP 4
#define FDT_END 9

static uint32_t be32(const void *p)
{
    const uint8_t *b = p;
    return (uint32_t)b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];
}

static uint64_t be64(const void *p) { return (uint64_t)be32(p) << 32 | be32((const uint8_t *)p + 4); }

#define ALIGN4(x) (((x) + 3) & ~3U)

/* Walks the structure block. cb is called for every property. */
typedef int (*prop_cb)(void *ctx, const char *node, const char *name, const uint8_t *val, uint32_t len);

static int walk(const void *fdt, prop_cb cb, void *ctx)
{
    const uint8_t *base = fdt;
    if (be32(base) != FDT_MAGIC) return -1;
    uint32_t off_struct = be32(base + 8), off_strings = be32(base + 12);
    const char *node = "";
    uint32_t off = off_struct;
    for (;;) {
        uint32_t tok = be32(base + off);
        off += 4;
        if (tok == FDT_BEGIN_NODE) {
            node = (const char *)(base + off);
            off += ALIGN4(strlen(node) + 1);
        } else if (tok == FDT_PROP) {
            uint32_t len = be32(base + off), nameoff = be32(base + off + 4);
            const char *name = (const char *)(base + off_strings + nameoff);
            int r = cb(ctx, node, name, base + off + 8, len);
            if (r) return r;
            off += 8 + ALIGN4(len);
        } else if (tok == FDT_END) {
            return 0;
        } else if (tok != FDT_END_NODE && tok != FDT_NOP) {
            return -1;
        }
    }
}

struct mem_ctx { uint64_t base, size; int found; };

static int mem_cb(void *c, const char *node, const char *name, const uint8_t *val, uint32_t len)
{
    struct mem_ctx *m = c;
    if (strncmp(node, "memory", 6) == 0 && strncmp(name, "reg", 4) == 0 && len >= 16) {
        m->base = be64(val);
        m->size = be64(val + 8);
        m->found = 1;
        return 1;  /* stop */
    }
    return 0;
}

int fdt_memory(const void *fdt, uint64_t *base, uint64_t *size)
{
    struct mem_ctx m = {0};
    int r = walk(fdt, mem_cb, &m);
    if (r < 0 || !m.found) return -1;
    *base = m.base;
    *size = m.size;
    return 0;
}
