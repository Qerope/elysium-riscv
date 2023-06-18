#include <stdint.h>

#include <ely/platform.h>
#include "../kern/fdt.h"

#define UART_IRQ 10

static volatile uint8_t *plic;
static int ctx;

#define PRIORITY(irq) (*(volatile uint32_t *)(plic + 4 * (irq)))
#define ENABLE(c) ((volatile uint32_t *)(plic + 0x2000 + 0x80 * (c)))
#define THRESHOLD(c) (*(volatile uint32_t *)(plic + 0x200000 + 0x1000 * (c)))
#define CLAIM(c) (*(volatile uint32_t *)(plic + 0x200004 + 0x1000 * (c)))

void uart_intr(void);

void plic_init(const void *fdt, uint64_t hart)
{
    uint64_t base, size;
    if (fdt_find_reg(fdt, "riscv,plic0", &base, &size))
        panic("no plic in device tree");
    plic = (volatile uint8_t *)mmio(base);
    ctx = 2 * hart + 1;  /* S-mode context of this hart */
    PRIORITY(UART_IRQ) = 1;
    ENABLE(ctx)[0] |= 1U << UART_IRQ;
    THRESHOLD(ctx) = 0;
}

void plic_intr(void)
{
    uint32_t irq = CLAIM(ctx);
    CLAIM(ctx) = irq;
    if (irq == UART_IRQ)
        uart_intr();
}
