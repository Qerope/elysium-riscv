#include <stdint.h>

#include <ely/platform.h>
#include "../kern/fdt.h"
#include <ely/sched.h>

#define RBR 0
#define THR 0
#define IER 1
#define LSR 5
#define LSR_RX_READY 0x01
#define LSR_TX_EMPTY 0x20

static volatile uint8_t *regs;
static char rxbuf[256];
static unsigned rx_head, rx_tail;

void uart_init(const void *fdt)
{
    uint64_t base, size;
    if (fdt_find_reg(fdt, "ns16550a", &base, &size))
        panic("no ns16550a in device tree");
    regs = (volatile uint8_t *)mmio(base);
    regs[IER] = 1;  /* receive data available */
}

void uart_putc(char c)
{
    while (!(regs[LSR] & LSR_TX_EMPTY)) {}
    regs[THR] = c;
}

void uart_intr(void)
{
    while (regs[LSR] & LSR_RX_READY) {
        char c = regs[RBR];
        if (rx_head - rx_tail < sizeof rxbuf)
            rxbuf[rx_head++ % sizeof rxbuf] = c;
    }
    thread_wakeup(rxbuf);
}

int uart_getc(void)
{
    unsigned long f = irq_save();
    while (rx_head == rx_tail)
        thread_block(rxbuf);
    char c = rxbuf[rx_tail++ % sizeof rxbuf];
    irq_restore(f);
    return c;
}
