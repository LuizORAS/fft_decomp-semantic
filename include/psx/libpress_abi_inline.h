#ifndef PSX_LIBPRESS_ABI_INLINE_H
#define PSX_LIBPRESS_ABI_INLINE_H

#include "psx/cpu_abi_inline.h"

/* Handwritten LIBPRESS CPU and return boundaries. */

/* Expand named immediate values through the two-level CPU stringifier. */
#define PSYQ_PRESS_TRAP_ADDI(result, a, imm)                                                                           \
    __asm__ volatile("addi %0, %1, " PSYQ_CPU_STRINGIFY(imm) : "=r"(result) : "r"(a))
#define PSYQ_PRESS_TRAP_CONSTANT(result, imm) __asm__ volatile("addi %0, $0, " PSYQ_CPU_STRINGIFY(imm) : "=r"(result))
/* This scratch-register transport uses OR; C zero assignment emits ADDU. */
#define PSYQ_PRESS_RESET_BIT_COUNT(count)                                                                              \
    __asm__ volatile(".ifnc %0,$3\n\t.error \"VLC count requires v1\"\n\t.endif\n\tor %0, $0, $0" : "=r"(count))
/* Read includes the original one-cycle COP0 wait; write has no trailing NOP. */
#define PSYQ_PRESS_STATUS_READ_WAIT(status) __asm__ volatile("mfc0 %0, $12; nop" : "=r"(status))
#define PSYQ_PRESS_STATUS_WRITE(status)     __asm__ volatile("mtc0 %0, $12" : : "r"(status))

/* Original terminal transport returns through v0/RA with trapping arithmetic. */
#define PSYQ_PRESS_RETURN_COMPLETE(window)                                                                             \
    __asm__ volatile(".ifnc %0,$2\n\t.error \"VLC return requires v0\"\n\t.endif\n\t.set\tnoreorder\n\t.macro j "      \
                     "architectural_ra\n\t.ifnc \\architectural_ra,$31\n\t"                                            \
                     ".error \"VLC return requires architectural RA\"\n\t.endif\n\tjr $31\n\tadd $2, $0, $0\n\t"       \
                     ".purgem j\n\t.endm\n\t"                                                                          \
        : "=r"(window))
#define PSYQ_PRESS_RETURN_PAUSED(window)                                                                               \
    __asm__ volatile(".ifnc %0,$2\n\t.error \"VLC return requires v0\"\n\t.endif\n\t.set\tnoreorder\n\t.macro j "      \
                     "architectural_ra\n\t.ifnc \\architectural_ra,$31\n\t"                                            \
                     ".error \"VLC return requires architectural RA\"\n\t.endif\n\tjr $31\n\taddi $2, $0, 1\n\t"       \
                     ".purgem j\n\t.endm\n\t"                                                                          \
        : "=r"(window))
#define PSYQ_PRESS_RETURN_END() __asm__(".set\treorder")

#endif
