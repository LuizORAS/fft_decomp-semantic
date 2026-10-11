#ifndef PSX_GTE_INLINE_SPLIT_H
#define PSX_GTE_INLINE_SPLIT_H

/* Split-statement GTE operations use CPU $12-$14 for three staged addresses.
 * The separate operand statements also retain the native input-register reuse
 * and allocation; combining them changes the callers' bytes. */
#define gte_ldv0_split(r0)                                                                                             \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("lwc2 $0, 0($12)");                                                                           \
        __asm__ volatile("lwc2 $1, 4($12)");                                                                           \
    }

#define gte_ldv3_split(r0, r1, r2)                                                                                     \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("addu $13, %0, $0" : : "r"(r1) : "$13");                                                      \
        __asm__ volatile("addu $14, %0, $0" : : "r"(r2) : "$14");                                                      \
        __asm__ volatile("lwc2 $0, 0($12)");                                                                           \
        __asm__ volatile("lwc2 $1, 4($12)");                                                                           \
        __asm__ volatile("lwc2 $2, 0($13)");                                                                           \
        __asm__ volatile("lwc2 $3, 4($13)");                                                                           \
        __asm__ volatile("lwc2 $4, 0($14)");                                                                           \
        __asm__ volatile("lwc2 $5, 4($14)");                                                                           \
    }

#define gte_rtps_split()                                                                                               \
    {                                                                                                                  \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile(".word 0x4a180001");                                                                          \
    }

#define gte_rtpt_split()                                                                                               \
    {                                                                                                                  \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile(".word 0x4a280030");                                                                          \
    }

#define gte_stsxy_split(r0)                                                                                            \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("swc2 $14, 0($12)" : : : "memory");                                                           \
    }

#define gte_stsxy3_split(r0, r1, r2)                                                                                   \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r0) : "$12");                                                      \
        __asm__ volatile("addu $13, %0, $0" : : "r"(r1) : "$13");                                                      \
        __asm__ volatile("addu $14, %0, $0" : : "r"(r2) : "$14");                                                      \
        __asm__ volatile("swc2 $12, 0($12)" : : : "memory");                                                           \
        __asm__ volatile("swc2 $13, 0($13)" : : : "memory");                                                           \
        __asm__ volatile("swc2 $14, 0($14)" : : : "memory");                                                           \
    }

#endif
