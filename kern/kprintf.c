#include <stdarg.h>
#include <stdint.h>

#include <ely/platform.h>
#include <arch/sbi.h>

static void putc_(char c)
{
    if (c == '\n') sbi_putchar('\r');
    sbi_putchar(c);
}

static void put_num(uint64_t v, unsigned base, int is_signed, int width)
{
    char buf[24];
    int n = 0, neg = 0;
    if (is_signed && (int64_t)v < 0) { neg = 1; v = -(int64_t)v; }
    do { buf[n++] = "0123456789abcdef"[v % base]; v /= base; } while (v);
    if (neg) buf[n++] = '-';
    while (width-- > n) putc_('0');
    while (n) putc_(buf[--n]);
}

void kprintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    for (; *fmt; fmt++) {
        if (*fmt != '%') { putc_(*fmt); continue; }
        switch (*++fmt) {
        case 's': { const char *s = va_arg(ap, const char *); while (s && *s) putc_(*s++); break; }
        case 'd': put_num(va_arg(ap, int), 10, 1, 0); break;
        case 'u': put_num(va_arg(ap, unsigned), 10, 0, 0); break;
        case 'x': put_num(va_arg(ap, unsigned), 16, 0, 0); break;
        case 'p': putc_('0'); putc_('x'); put_num((uint64_t)va_arg(ap, void *), 16, 0, 16); break;
        case '%': putc_('%'); break;
        }
    }
    va_end(ap);
}

void panic(const char *fmt, ...)
{
    kprintf("panic: ");
    kprintf("%s\n", fmt);
    for (;;) __asm__ volatile("wfi");
}
