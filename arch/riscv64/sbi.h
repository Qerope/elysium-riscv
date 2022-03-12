#pragma once
#include <stdint.h>

struct sbiret { long error, value; };

static inline struct sbiret sbi_ecall(long eid, long fid, long a0, long a1, long a2) {
    register long r0 __asm__("a0") = a0;
    register long r1 __asm__("a1") = a1;
    register long r2 __asm__("a2") = a2;
    register long r6 __asm__("a6") = fid;
    register long r7 __asm__("a7") = eid;
    __asm__ volatile("ecall" : "+r"(r0), "+r"(r1) : "r"(r2), "r"(r6), "r"(r7) : "memory");
    return (struct sbiret){r0, r1};
}

#define SBI_EXT_BASE 0x10
#define SBI_EXT_TIME 0x54494D45
#define SBI_EXT_HSM 0x48534D

static inline void sbi_putchar_legacy(int c) { sbi_ecall(1, 0, c, 0, 0); }
static inline void sbi_set_timer(uint64_t t) { sbi_ecall(SBI_EXT_TIME, 0, (long)t, 0, 0); }
static inline struct sbiret sbi_hart_start(long hart, long addr, long opaque) {
    return sbi_ecall(SBI_EXT_HSM, 0, hart, addr, opaque);
}
#define sbi_putchar sbi_putchar_legacy
