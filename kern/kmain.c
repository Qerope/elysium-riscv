#include <stdint.h>

#include <arch/sbi.h>

void kmain(uint64_t hartid, uint64_t dtb)
{
    (void)hartid; (void)dtb;
    const char *s = "Elysium\n";
    while (*s) sbi_putchar_legacy(*s++);
    for (;;) __asm__ volatile("wfi");
}
