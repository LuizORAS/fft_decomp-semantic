#ifndef PSX_CPU_RETURN_ZERO_INLINE_H
#define PSX_CPU_RETURN_ZERO_INLINE_H
/* Return zero through architectural v0 and RA. GCC2.6 emits a C computed
 * transfer as `j $31`; a one-shot assembler pseudo-op supplies its required
 * ADDIU zero delay slot without another C epilogue. Only this terminal transfer
 * is accepted. The macro purges itself; no generated instruction is discarded. */
#define PSYQ_CPU_RETURN_ZERO()                                                                                         \
    do {                                                                                                               \
        __asm__ volatile(".ifnc %0,$2\n\t.error \"CPU zero result requires architectural "                             \
                         "v0\"\n\t.endif\n\t.set\tnoreorder\n\t.macro j architectural_ra\n\t.ifnc "                    \
                         "\\architectural_ra,$31\n\t.error \"CPU zero return requires architectural "                  \
                         "RA\"\n\t.endif\n\tjr \\architectural_ra\n\taddiu $2, $0, 0\n\t.purgem j\n\t.endm"            \
            : "=r"(psyq_cpu_return_value));                                                                            \
        goto* psyq_cpu_return_address;                                                                                 \
    } while (0)
/* File-scope metadata restores assembler scheduling after the final boundary. */
#define PSYQ_CPU_RETURN_ZERO_END() __asm__(".set\treorder")
#endif
