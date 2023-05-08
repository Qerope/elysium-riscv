#include <stdint.h>

#include <ely/platform.h>
#include "../kern/fdt.h"

#define RBR 0
#define THR 0
#define IER 1
#define LSR 5
#define LSR_RX_READY 0x01
#define LSR_TX_EMPTY 0x20

static volatile uint8_t *regs;

void uart_init(const void *fdt)
{
    uint64_t base, size;
    if (fdt_find_reg(fdt, "ns16550a", &base, &size))
        panic("no ns16550a in device tree");
    regs = (volatile uint8_t *)mmio(base);
}

void uart_putc(char c)
{
    while (!(regs[LSR] & LSR_TX_EMPTY)) {}
    regs[THR] = c;
}
