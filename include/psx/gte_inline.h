#ifndef PSX_GTE_INLINE_H
#define PSX_GTE_INLINE_H

#include "psx/bios.h"
#include "psx/cpu_abi_inline.h"
#include "psx/gte_regs.h"

#define PSYQ_GTE_STRINGIFY_VALUE(value) #value
#define PSYQ_GTE_STRINGIFY(value)       PSYQ_GTE_STRINGIFY_VALUE(value)

/* Clean-room GTE (COP2) operation macros.
 *
 * The retail game code issues GTE operations through the SDK's inline macros
 * in their DMPSX form, which C cannot express: every memory operand is first
 * copied to $12, and each GTE command is preceded by two nops. These macros
 * reproduce the instruction sequences observed in the target (for example the
 * EFFECT mesh renderers' ldv0/rtv0tr/stlvnl/stflg sequence) and keep the SDK's
 * names. Add an operation only with target evidence of its exact form. */

/* Load V0 from an SVECTOR. */
#define gte_ldv0(r0)                                                                                                   \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "lwc2 $0, 0($12);"                                                                                \
                     "lwc2 $1, 4($12)"                                                                                 \
        :                                                                                                              \
        : "r"(r0)                                                                                                      \
        : "$12")

/* Load V0, V1 and V2 from three SVECTORs. */
#define gte_ldv3(r0, r1, r2)                                                                                           \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "lwc2 $0, 0($12);"                                                                                \
                     "lwc2 $1, 4($12);"                                                                                \
                     "addu $12, %1, $0;"                                                                               \
                     "lwc2 $2, 0($12);"                                                                                \
                     "lwc2 $3, 4($12);"                                                                                \
                     "addu $12, %2, $0;"                                                                               \
                     "lwc2 $4, 0($12);"                                                                                \
                     "lwc2 $5, 4($12)"                                                                                 \
        :                                                                                                              \
        : "r"(r0), "r"(r1), "r"(r2)                                                                                    \
        : "$12")

/* MVMVA: rotate V0 by the rotation matrix and add the translation vector. */
#define gte_rtv0tr()                                                                                                   \
    __asm__ volatile("nop;"                                                                                            \
                     "nop;"                                                                                            \
                     ".word 0x4a480012")

/* RTPS: perspective-transform V0. */
#define gte_rtps()                                                                                                     \
    __asm__ volatile("nop;"                                                                                            \
                     "nop;"                                                                                            \
                     ".word 0x4a180001")

/* RTPT: perspective-transform V0, V1 and V2. */
#define gte_rtpt()                                                                                                     \
    __asm__ volatile("nop;"                                                                                            \
                     "nop;"                                                                                            \
                     ".word 0x4a280030")

/* Store MAC1-MAC3 to a VECTOR's three words. */
#define gte_stlvnl(r0)                                                                                                 \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "swc2 $25, 0($12);"                                                                               \
                     "swc2 $26, 4($12);"                                                                               \
                     "swc2 $27, 8($12)"                                                                                \
        :                                                                                                              \
        : "r"(r0)                                                                                                      \
        : "$12", "memory")

/* Store the FLAG control register to a word. */
#define gte_stflg(r0)                                                                                                  \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "cfc2 $13, $31;"                                                                                  \
                     "nop;"                                                                                            \
                     "sw $13, 0($12)"                                                                                  \
        :                                                                                                              \
        : "r"(r0)                                                                                                      \
        : "$12", "$13", "memory")

/* Store SXY2 (the last projected screen coordinate). */
#define gte_stsxy(r0)                                                                                                  \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "swc2 $14, 0($12)"                                                                                \
        :                                                                                                              \
        : "r"(r0)                                                                                                      \
        : "$12", "memory")

/* Store SXY0, SXY1 and SXY2. */
#define gte_stsxy3(r0, r1, r2)                                                                                         \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "swc2 $12, 0($12);"                                                                               \
                     "addu $12, %1, $0;"                                                                               \
                     "swc2 $13, 0($12);"                                                                               \
                     "addu $12, %2, $0;"                                                                               \
                     "swc2 $14, 0($12)"                                                                                \
        :                                                                                                              \
        : "r"(r0), "r"(r1), "r"(r2)                                                                                    \
        : "$12", "memory")

/* RotTrans: V0 = *r1, then store the rotated and translated MAC1-MAC3 to r2
 * and FLAG to r3. The empty clobber of $14/$15 reproduces the retail register
 * allocation around the sequence. */
#define gte_RotTrans(r1, r2, r3)                                                                                       \
    {                                                                                                                  \
        gte_ldv0(r1);                                                                                                  \
        gte_rtv0tr();                                                                                                  \
        gte_stlvnl(r2);                                                                                                \
        gte_stflg(r3);                                                                                                 \
        __asm__ volatile("" : : : "$14", "$15");                                                                       \
    }

/* The same RotTrans issued as one statement per instruction. Its larger RTL
 * insn count changes GCC's loop-invariant and strength-reduction decisions;
 * the 5,368-byte EFFECT ring renderers need this form. */
#define gte_RotTrans_split(r1, r2, r3)                                                                                 \
    {                                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r1) : "$12");                                                      \
        __asm__ volatile("lwc2 $0, 0($12)");                                                                           \
        __asm__ volatile("lwc2 $1, 4($12)");                                                                           \
        gte_rtv0tr();                                                                                                  \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r2) : "$12");                                                      \
        __asm__ volatile("swc2 $25, 0($12)" : : : "memory");                                                           \
        __asm__ volatile("swc2 $26, 4($12)" : : : "memory");                                                           \
        __asm__ volatile("swc2 $27, 8($12)" : : : "memory");                                                           \
        __asm__ volatile("addu $12, %0, $0" : : "r"(r3) : "$12");                                                      \
        __asm__ volatile("cfc2 $13, $31" : : : "$13");                                                                 \
        __asm__ volatile("nop");                                                                                       \
        __asm__ volatile("sw $13, 0($12)" : : : "memory");                                                             \
        __asm__ volatile("" : : : "$14", "$15");                                                                       \
    }

/* Load the rotation matrix (control registers 0-4) from a MATRIX. The
 * "memory" clobber makes GCC reload pointers read after the macro. */
#define gte_SetRotMatrix(r)                                                                                            \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "lw $13, 0($12);"                                                                                 \
                     "lw $14, 4($12);"                                                                                 \
                     "ctc2 $13, $0;"                                                                                   \
                     "ctc2 $14, $1;"                                                                                   \
                     "lw $13, 8($12);"                                                                                 \
                     "lw $14, 12($12);"                                                                                \
                     "lw $15, 16($12);"                                                                                \
                     "ctc2 $13, $2;"                                                                                   \
                     "ctc2 $14, $3;"                                                                                   \
                     "ctc2 $15, $4"                                                                                    \
        :                                                                                                              \
        : "r"(r)                                                                                                       \
        : "$12", "$13", "$14", "$15", "memory")

/* Load the translation vector (control registers 5-7) from a MATRIX. */
#define gte_SetTransMatrix(r)                                                                                          \
    __asm__ volatile("addu $12, %0, $0;"                                                                               \
                     "lw $13, 20($12);"                                                                                \
                     "lw $14, 24($12);"                                                                                \
                     "ctc2 $13, $5;"                                                                                   \
                     "lw $15, 28($12);"                                                                                \
                     "ctc2 $14, $6;"                                                                                   \
                     "ctc2 $15, $7"                                                                                    \
        :                                                                                                              \
        : "r"(r)                                                                                                       \
        : "$12", "$13", "$14", "$15", "memory")

/* Linked LIBGTE uses direct COP2 operands rather than the game DMPSX form. */
#define PSYQ_GTE_CTC2(reg, value) __asm__ volatile("ctc2 %0, $" PSYQ_GTE_STRINGIFY(reg) : : "r"(value))
#define PSYQ_GTE_CFC2(reg, value) __asm__ volatile("cfc2 %0, $" PSYQ_GTE_STRINGIFY(reg) : "=r"(value))
#define PSYQ_GTE_LZCS(value, result)                                                                                   \
    __asm__ volatile("mtc2 %1, $30; nop; nop; mfc2 %0, $31" : "=r"(result) : "r"(value))
#define PSYQ_GTE_ASM_CTC2(cpu_reg, control_reg)                                                                        \
    "ctc2 $" PSYQ_GTE_STRINGIFY(cpu_reg) ", $" PSYQ_GTE_STRINGIFY(control_reg) ";"
#define PSYQ_GTE_ASM_CFC2(cpu_reg, control_reg)                                                                        \
    "cfc2 $" PSYQ_GTE_STRINGIFY(cpu_reg) ", $" PSYQ_GTE_STRINGIFY(control_reg) ";"
#define PSYQ_GTE_LOAD_MATRIX(matrix, reg0, reg1, reg2, reg3, reg4)                                                     \
    __asm__ volatile("lw $8, 0(%0); lw $9, 4(%0); lw $10, 8(%0); lw $11, 12(%0); lw $12, 16(%0);" PSYQ_GTE_ASM_CTC2(   \
        8, reg0) PSYQ_GTE_ASM_CTC2(9, reg1) PSYQ_GTE_ASM_CTC2(10, reg2) PSYQ_GTE_ASM_CTC2(11, reg3)                    \
            PSYQ_GTE_ASM_CTC2(12, reg4)                                                                                \
        :                                                                                                              \
        : "r"(matrix)                                                                                                  \
        : "$8", "$9", "$10", "$11", "$12", "memory")
#define PSYQ_GTE_LOAD_TRANSLATION(matrix)                                                                              \
    __asm__ volatile("lw $8, 20(%0); lw $9, 24(%0); lw $10, 28(%0);"                                                   \
                     "ctc2 $8, $5; ctc2 $9, $6; ctc2 $10, $7"                                                          \
        :                                                                                                              \
        : "r"(matrix)                                                                                                  \
        : "$8", "$9", "$10", "memory")
#define PSYQ_GTE_READ_MATRIX(matrix, reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7)                                   \
    __asm__ volatile(PSYQ_GTE_ASM_CFC2(8, reg0) PSYQ_GTE_ASM_CFC2(9, reg1) PSYQ_GTE_ASM_CFC2(                          \
        10, reg2) PSYQ_GTE_ASM_CFC2(11, reg3) PSYQ_GTE_ASM_CFC2(12,                                                    \
        reg4) "sw $8, 0(%0); sw $9, 4(%0); sw $10, 8(%0); sw $11, 12(%0); sw $12, 16(%0);" PSYQ_GTE_ASM_CFC2(8, reg5)  \
            PSYQ_GTE_ASM_CFC2(9, reg6) PSYQ_GTE_ASM_CFC2(10, reg7) "sw $8, 20(%0); sw $9, 24(%0); sw $10, 28(%0)"      \
        :                                                                                                              \
        : "r"(matrix)                                                                                                  \
        : "$8", "$9", "$10", "$11", "$12", "memory")
#define PSYQ_GTE_ROTTRANS(input, output, flag)                                                                         \
    __asm__ volatile("lwc2 $0, 0(%1); lwc2 $1, 4(%1); nop; .word 0x4a480012;"                                          \
                     "swc2 $25, 0(%2); swc2 $26, 4(%2); swc2 $27, 8(%2); cfc2 %0, $31"                                 \
        : "=r"(flag)                                                                                                   \
        : "r"(input), "r"(output)                                                                                      \
        : "memory")
#define PSYQ_GTE_NORMALCLIP(a, b, c, result)                                                                           \
    __asm__ volatile("mtc2 %1, $12; mtc2 %3, $14; mtc2 %2, $13;"                                                       \
                     "nop; nop; .word 0x4b400006; mfc2 %0, $24"                                                        \
        : "=r"(result)                                                                                                 \
        : "r"(a), "r"(b), "r"(c))
#define PSYQ_GTE_IR_VECTOR(input, output, command, reg0, reg1, reg2)                                                   \
    __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0);"                                               \
                     "nop; .word " PSYQ_GTE_STRINGIFY(command) "; swc2 $" PSYQ_GTE_STRINGIFY(                          \
                         reg0) ", 0(%1);"                                                                              \
                               "swc2 $" PSYQ_GTE_STRINGIFY(reg1) ", 4(%1); swc2 $" PSYQ_GTE_STRINGIFY(reg2) ", 8(%1)"  \
        :                                                                                                              \
        : "r"(input), "r"(output)                                                                                      \
        : "memory")
#define PSYQ_GTE_ROTTRANSSV(input, output, flag)                                                                       \
    __asm__ volatile("lwc2 $0, 0(%1); lwc2 $1, 4(%1); nop; .word 0x4a480012;"                                          \
                     "mfc2 $2, $9; mfc2 $3, $10; swc2 $11, 4(%2);"                                                     \
                     "sh $2, 0(%2); sh $3, 2(%2); cfc2 %0, $31"                                                        \
        : "=r"(flag)                                                                                                   \
        : "r"(input), "r"(output)                                                                                      \
        : "$3", "memory")
#define PSYQ_GTE_SVECTOR_SQUARE(input, output, command)                                                                \
    __asm__ volatile("lh $2, 0(%0); lh $3, 2(%0); mtc2 $2, $9; mtc2 $3, $10;"                                          \
                     "lwc2 $11, 4(%0); nop; .word " PSYQ_GTE_STRINGIFY(                                                \
                         command) ";"                                                                                  \
                                  "mfc2 $2, $9; mfc2 $3, $10; swc2 $11, 4(%1); sh $2, 0(%1); sh $3, 2(%1)"             \
        :                                                                                                              \
        : "r"(input), "r"(output)                                                                                      \
        : "$2", "$3", "memory")
#define PSYQ_GTE_SVECTOR_SQUARE_LONG(input, output, command)                                                           \
    __asm__ volatile(                                                                                                  \
        "lh $2, 0(%0); lh $3, 2(%0); mtc2 $2, $9; mtc2 $3, $10;"                                                       \
        "lwc2 $11, 4(%0); nop; .word " PSYQ_GTE_STRINGIFY(command) ";"                                                 \
                                                                   "swc2 $9, 0(%1); swc2 $10, 4(%1); swc2 $11, 8(%1)"  \
        :                                                                                                              \
        : "r"(input), "r"(output)                                                                                      \
        : "$2", "$3", "memory")
#define PSYQ_GTE_AVSZ3(z0, z1, z2, result)                                                                             \
    __asm__ volatile("mtc2 %1, $17; mtc2 %2, $18; mtc2 %3, $19;"                                                       \
                     "nop; .word 0x4b58002d; mfc2 %0, $7"                                                              \
        : "=r"(result)                                                                                                 \
        : "r"(z0), "r"(z1), "r"(z2))
#define PSYQ_GTE_AVSZ4(z0, z1, z2, z3, result)                                                                         \
    __asm__ volatile("mtc2 %1, $16; mtc2 %2, $17; mtc2 %3, $18; mtc2 %4, $19;"                                         \
                     "nop; .word 0x4b68002e; mfc2 %0, $7"                                                              \
        : "=r"(result)                                                                                                 \
        : "r"(z0), "r"(z1), "r"(z2), "r"(z3))
#define PSYQ_GTE_OUTER_PRODUCT(a, b, output, command)                                                                  \
    __asm__ volatile("cfc2 $13, $0; cfc2 $14, $2; cfc2 $15, $4;"                                                       \
                     "lw $8, 0(%0); lw $9, 4(%0); lw $10, 8(%0);"                                                      \
                     "ctc2 $8, $0; ctc2 $9, $2; ctc2 $10, $4;"                                                         \
                     "lwc2 $11, 8(%1); lwc2 $9, 0(%1); lwc2 $10, 4(%1); nop;"                                          \
                     ".word " PSYQ_GTE_STRINGIFY(command) "; swc2 $25, 0(%2); swc2 $26, 4(%2); swc2 $27, 8(%2);"       \
                                                          "ctc2 $13, $0; ctc2 $14, $2; ctc2 $15, $4"                   \
        :                                                                                                              \
        : "r"(a), "r"(b), "r"(output)                                                                                  \
        : "$8", "$9", "$10", "$13", "$14", "$15", "memory")
#define PSYQ_GTE_INTPLC(input, depth, output)                                                                          \
    __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0);"                                               \
                     "mtc2 %1, $8; nop; .word 0x4a980011; swc2 $22, 0(%2)"                                             \
        :                                                                                                              \
        : "r"(input), "r"(depth), "r"(output)                                                                          \
        : "memory")
#define PSYQ_GTE_DCPL(input, color, depth, output)                                                                     \
    __asm__ volatile("lwc2 $9, 0(%0); lwc2 $10, 4(%0); lwc2 $11, 8(%0);"                                               \
                     "lwc2 $6, 0(%1); mtc2 %2, $8; nop; .word 0x4a680029; swc2 $22, 0(%3)"                             \
        :                                                                                                              \
        : "r"(input), "r"(color), "r"(depth), "r"(output)                                                              \
        : "memory")

/* Projection and depth-cue entry points use the original o32 argument slots. */
#define PSYQ_GTE_ROTPERSP(input, xy, p, flag, depth)                                                                   \
    __asm__ volatile("lwc2 $0, 0(%1); lwc2 $1, 4(%1); nop; .word 0x4a180001;"                                          \
                     "swc2 $14, 0(%2); swc2 $8, 0(%3); cfc2 $3, $31;"                                                  \
                     "mfc2 %0, $19; sw $3, 0(%4)"                                                                      \
        : "=r"(depth)                                                                                                  \
        : "r"(input), "r"(xy), "r"(p), "r"(flag)                                                                       \
        : "$3", "memory")
#define PSYQ_GTE_ROTPERSP3(v0, v1, v2, xy0, xy1, xy2, p, flag, depth)                                                  \
    __asm__ volatile("lwc2 $0, 0(%1); lwc2 $1, 4(%1);"                                                                 \
                     "lwc2 $2, 0(%2); lwc2 $3, 4(%2); lwc2 $4, 0(%3); lwc2 $5, 4(%3);"                                 \
                     "nop; .word 0x4a280030; lw $8, %5; lw $9, %6; lw $10, %7; lw $11, %8;"                            \
                     "swc2 $12, 0(%4); swc2 $13, 0($8); swc2 $14, 0($9); swc2 $8, 0($10);"                             \
                     "cfc2 $3, $31; mfc2 %0, $19; sw $3, 0($11)"                                                       \
        : "=r"(depth)                                                                                                  \
        : "r"(v0), "r"(v1), "r"(v2), "r"(xy0), "m"(xy1), "m"(xy2), "m"(p), "m"(flag)                                   \
        : "$3", "$8", "$9", "$10", "$11", "memory")
#define PSYQ_GTE_DPCT(c0, c1, c2, depth, out0, out1, out2)                                                             \
    __asm__ volatile("lwc2 $20, 0(%0); lwc2 $21, 0(%1); lwc2 $22, 0(%2);"                                              \
                     "lwc2 $6, 0(%2); mtc2 %3, $8; nop; .word 0x4af8002a;"                                             \
                     "lw $8, %4; lw $9, %5; lw $10, %6;"                                                               \
                     "swc2 $20, 0($8); swc2 $21, 0($9); swc2 $22, 0($10)"                                              \
        :                                                                                                              \
        : "r"(c0), "r"(c1), "r"(c2), "r"(depth), "m"(out0), "m"(out1), "m"(out2)                                       \
        : "$8", "$9", "$10", "memory")

#define PSYQ_GTE_ROTPERSP4_FIRST(v0, v1, v2, xy0, xy1, xy2, first_flag)                                                \
    __asm__ volatile("lwc2 $0, 0(%1); lwc2 $1, 4(%1);"                                                                 \
                     "lwc2 $2, 0(%2); lwc2 $3, 4(%2); lwc2 $4, 0(%3); lwc2 $5, 4(%3);"                                 \
                     "nop; .word 0x4a280030; lw $8, %4; lw $9, %5; lw $10, %6;"                                        \
                     "swc2 $12, 0($8); swc2 $13, 0($9); swc2 $14, 0($10); cfc2 %0, $31"                                \
        : "=r"(first_flag)                                                                                             \
        : "r"(v0), "r"(v1), "r"(v2), "m"(xy0), "m"(xy1), "m"(xy2)                                                      \
        : "$8", "$9", "$10", "memory")

#define PSYQ_GTE_ROTPERSP4_LAST(v3, xy3, p, flag, first_flag, depth)                                                   \
    __asm__ volatile("lwc2 $0, 0(%1); lwc2 $1, 4(%1); nop; .word 0x4a180001;"                                          \
                     "lw $8, %2; lw $9, %3; lw $10, %4; swc2 $14, 0($8); swc2 $8, 0($9);"                              \
                     "cfc2 $8, $31; mfc2 %0, $19; or $8, $8, %5; sw $8, 0($10)"                                        \
        : "=r"(depth)                                                                                                  \
        : "r"(v3), "m"(xy3), "m"(p), "m"(flag), "r"(first_flag)                                                        \
        : "$8", "$9", "$10", "memory")

/* Load each matrix column into V0 and preserve IR results until all input
 * columns have been read. This ordering also preserves overlapping outputs. */
#define PSYQ_GTE_MATRIX_COLUMNS_FORM(right, load_first, mask_first)                                                    \
    __asm__ volatile(load_first " $8, 0(%0); lw $9, 4(%0); lw $10, 12(%0);" mask_first                                 \
                                "lui $1, 0xffff; and $9, $9, $1; or $8, $8, $9;"                                       \
                                "mtc2 $8, $0; mtc2 $10, $1; nop; .word 0x4a486012;"                                    \
                                "lhu $8, 2(%0); lw $9, 8(%0); lh $10, 14(%0);"                                         \
                                "sll $9, $9, 16; or $8, $8, $9; mfc2 $11, $9; mfc2 $12, $10; mfc2 $13, $11;"           \
                                "mtc2 $8, $0; mtc2 $10, $1; nop; .word 0x4a486012;"                                    \
                                "lhu $8, 4(%0); lw $9, 8(%0); lw $10, 16(%0);"                                         \
                                "lui $1, 0xffff; and $9, $9, $1; or $8, $8, $9;"                                       \
                                "mfc2 $14, $9; mfc2 $15, $10; mfc2 $24, $11;"                                          \
                                "mtc2 $8, $0; mtc2 $10, $1; nop; .word 0x4a486012"                                     \
        :                                                                                                              \
        : "r"(right)                                                                                                   \
        : "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "memory")
#define PSYQ_GTE_MATRIX_COLUMNS(right)      PSYQ_GTE_MATRIX_COLUMNS_FORM(right, "lhu", "")
#define PSYQ_GTE_MATRIX_COLUMNS_WORD(right) PSYQ_GTE_MATRIX_COLUMNS_FORM(right, "lw", "andi $8, $8, 0xffff;")
#define PSYQ_GTE_MATRIX_STORE(output)                                                                                  \
    __asm__ volatile("andi $11, $11, 0xffff; sll $14, $14, 16; or $14, $14, $11;"                                      \
                     "sw $14, 0(%0); andi $13, $13, 0xffff; sll $24, $24, 16; or $24, $24, $13;"                       \
                     "sw $24, 12(%0); mfc2 $8, $9; mfc2 $9, $10; andi $8, $8, 0xffff;"                                 \
                     "sll $12, $12, 16; or $8, $8, $12; sw $8, 4(%0);"                                                 \
                     "andi $15, $15, 0xffff; sll $9, $9, 16; or $9, $9, $15;"                                          \
                     "sw $9, 8(%0); swc2 $11, 16(%0)"                                                                  \
        :                                                                                                              \
        : "r"(output)                                                                                                  \
        : "$8", "$9", "$11", "$12", "$13", "$14", "$15", "$24", "memory")
#define PSYQ_GTE_MATRIX_TO_CONTROL()                                                                                   \
    __asm__ volatile("andi $11, $11, 0xffff; sll $14, $14, 16; or $14, $14, $11;"                                      \
                     "andi $13, $13, 0xffff; sll $24, $24, 16; or $24, $24, $13;"                                      \
                     "mfc2 $8, $9; mfc2 $9, $10; mfc2 $10, $11; andi $8, $8, 0xffff;"                                  \
                     "sll $12, $12, 16; or $8, $8, $12; andi $15, $15, 0xffff;"                                        \
                     "sll $9, $9, 16; or $9, $9, $15;"                                                                 \
                     "ctc2 $14, $0; ctc2 $8, $1; ctc2 $9, $2; ctc2 $24, $3; ctc2 $10, $4"                              \
        :                                                                                                              \
        :                                                                                                              \
        : "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24")

/* Decomposed wide-vector MVMVA uses IR rather than the signed16 V0 inputs. */
#define PSYQ_GTE_APPLY_LONG_HIGH(x, y, z)                                                                              \
    __asm__ volatile("mtc2 %0, $9; mtc2 %1, $10; mtc2 %2, $11;"                                                        \
                     "nop; .word 0x4a41e012; mfc2 %0, $25; mfc2 %1, $26; mfc2 %2, $27"                                 \
        : "=r"(x), "=r"(y), "=r"(z)                                                                                    \
        : "0"(x), "1"(y), "2"(z))
#define PSYQ_GTE_APPLY_LONG_LOW(x, y, z)                                                                               \
    __asm__ volatile("mtc2 %0, $9; mtc2 %1, $10; mtc2 %2, $11;"                                                        \
                     "nop; .word 0x4a49e012"                                                                           \
        :                                                                                                              \
        : "r"(x), "r"(y), "r"(z))
#define PSYQ_GTE_READ_MAC(x, y, z)                                                                                     \
    __asm__ volatile("mfc2 %0, $25; mfc2 %1, $26; mfc2 %2, $27" : "=r"(x), "=r"(y), "=r"(z))
#define PSYQ_GTE_APPLY_SHORT(input, output)                                                                            \
    __asm__ volatile("lw $8, 0(%0); lw $9, 4(%0); mtc2 $8, $0; mtc2 $9, $1;"                                           \
                     "nop; .word 0x4a486012; swc2 $9, 0(%1); swc2 $10, 4(%1); swc2 $11, 8(%1)"                         \
        :                                                                                                              \
        : "r"(input), "r"(output)                                                                                      \
        : "$8", "$9", "memory")

/* Retail uses MULTU for low-word products; GCC 2.6.3 lowers C products to MULT. */
#define PSYQ_GTE_MULTU_BEGIN(a, b) __asm__ volatile("multu %0, %1" : : "r"(a), "r"(b) : "hi", "lo", "memory")
#define PSYQ_GTE_READ_LO(result)   __asm__ volatile("mflo %0" : "=r"(result) : : "memory")

/* Paired relocation metadata binds the typed C LH one ADDU after the LUI.
 * Normal byte checks enforce this fixed scratch-register layout. */
#define PSYQ_GTE_TABLE_HIGH_PAIRED(result, table)                                                                      \
    __asm__("lui %0, %%hi(%1); .reloc .+4, R_MIPS_LO16, %1" : "=r"(result) : "i"(table))

/* Pack the matrix IR results while the translation MVMVA is in flight. */
#define PSYQ_GTE_COMPOSE_STORE(output, right)                                                                          \
    __asm__ volatile("andi $11, $11, 0xffff; sll $14, $14, 16; or $14, $14, $11;"                                      \
                     "sw $14, 0(%0); andi $13, $13, 0xffff; sll $24, $24, 16; or $24, $24, $13;"                       \
                     "sw $24, 12(%0); mfc2 $8, $9; mfc2 $9, $10; swc2 $11, 16(%0);"                                    \
                     "lhu $13, 20(%1); lw $14, 24(%1); lw $10, 28(%1); sll $14, $14, 16; or $13, $13, $14;"            \
                     "mtc2 $13, $0; mtc2 $10, $1; nop; .word 0x4a486012;"                                              \
                     "sll $12, $12, 16; andi $8, $8, 0xffff; or $8, $8, $12; sw $8, 4(%0);"                            \
                     "andi $15, $15, 0xffff; sll $9, $9, 16; or $9, $9, $15; sw $9, 8(%0)"                             \
        :                                                                                                              \
        : "r"(output), "r"(right)                                                                                      \
        : "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "memory")

#define PSYQ_GTE_NORMALIZE_SQUARE(x, y, z, sx, sy, sz)                                                                 \
    __asm__ volatile("mtc2 %3, $9; mtc2 %4, $10; mtc2 %5, $11; nop; .word 0x4aa00428;"                                 \
                     "mfc2 %0, $25; mfc2 %1, $26; mfc2 %2, $27"                                                        \
        : "=r"(sx), "=r"(sy), "=r"(sz)                                                                                 \
        : "r"(x), "r"(y), "r"(z))
#define PSYQ_GTE_NORMALIZE_SCALE(x, y, z, factor)                                                                      \
    __asm__ volatile("mtc2 %6, $8; mtc2 %3, $9; mtc2 %4, $10; mtc2 %5, $11;"                                           \
                     "nop; nop; .word 0x4b90003d; mfc2 %0, $25; mfc2 %1, $26; mfc2 %2, $27"                            \
        : "=r"(x), "=r"(y), "=r"(z)                                                                                    \
        : "0"(x), "1"(y), "2"(z), "r"(factor))

/* These entries own RA in a3; a normal C call would add a stack frame.
 * The transport restores RA before the C epilogue and exposes the worker's
 * actual register inputs, outputs and clobbers. */
#define PSYQ_GTE_SAVE_RA(saved)       __asm__ volatile("addu %0, $31, $0" : "=r"(saved))
#define PSYQ_GTE_SAVE_RA_SHORT(saved) __asm__ volatile("nop; addu %0, $31, $0" : "=r"(saved))
#define PSYQ_GTE_RESTORE_RA(saved)    __asm__ volatile("addu $31, %0, $0" : : "r"(saved))
#define PSYQ_GTE_NORMALIZE_CALL(worker, x, y, z, sum)                                                                  \
    __asm__ volatile("jal %4"                                                                                          \
        : "=r"(x), "=r"(y), "=r"(z), "=r"(sum)                                                                         \
        : "i"(worker), "0"(x), "1"(y), "2"(z)                                                                          \
        : "$1", "$3", "$11", "$12", "$13", "$14", "memory")

#define PSYQ_GTE_READ_ROT_DIAGONAL(a, b, c)                                                                            \
    __asm__ volatile("cfc2 %0, $0; cfc2 %1, $2; cfc2 %2, $4" : "=r"(a), "=r"(b), "=r"(c))
#define PSYQ_GTE_RESTORE_ROT_DIAGONAL(a, b, c)                                                                         \
    __asm__ volatile("ctc2 %0, $0; ctc2 %1, $2; ctc2 %2, $4" : : "r"(a), "r"(b), "r"(c))
#define PSYQ_GTE_NORMAL_FIRST_CROSS(x, y, z, bx, by, bz, cx, cy, cz)                                                   \
    __asm__ volatile("ctc2 %3, $0; ctc2 %4, $2; ctc2 %5, $4;"                                                          \
                     "mtc2 %8, $11; mtc2 %6, $9; mtc2 %7, $10; nop; .word 0x4b78000c;"                                 \
                     "mfc2 %0, $25; mfc2 %1, $26; mfc2 %2, $27"                                                        \
        : "=r"(cx), "=r"(cy), "=r"(cz)                                                                                 \
        : "r"(x), "r"(y), "r"(z), "r"(bx), "r"(by), "r"(bz))
#define PSYQ_GTE_NORMAL_SECOND_CROSS(x, y, z, bx, by, bz)                                                              \
    __asm__ volatile("ctc2 %3, $0; ctc2 %4, $2; ctc2 %5, $4; nop; .word 0x4b78000c;"                                   \
                     "mtc2 %3, $0; mtc2 %4, $1; mtc2 %5, $2;"                                                          \
                     "mfc2 %0, $25; mfc2 %1, $26; mfc2 %2, $27"                                                        \
        : "=r"(x), "=r"(y), "=r"(z)                                                                                    \
        : "r"(bx), "r"(by), "r"(bz))
#define PSYQ_GTE_NORMAL_READ_SECOND(x, y, z)                                                                           \
    __asm__ volatile("mfc2 %0, $0; mfc2 %1, $1; mfc2 %2, $2" : "=r"(x), "=r"(y), "=r"(z))

/* Retail moves the third scratch argument in the call's delay slot. */
#define PSYQ_GTE_NORMALIZE_CALL_Z(worker, x, y, z, sum, third_z)                                                       \
    __asm__ volatile(".set\tnoreorder\n\tjal %4\n\taddu %2, %7, $0\n\t.set\treorder"                                   \
        : "=r"(x), "=r"(y), "=r"(z), "=r"(sum)                                                                         \
        : "i"(worker), "0"(x), "1"(y), "r"(third_z)                                                                    \
        : "$1", "$3", "$11", "$12", "$13", "$14", "memory")

#define PSYQ_GTE_CONTROL_WRITE_NOP(value, index)                                                                       \
    __asm__ volatile("ctc2 %0, $" PSYQ_GTE_STRINGIFY(index) "; nop" : : "r"(value))
#define PSYQ_GTE_ZERO_OFFSETS() __asm__ volatile("ctc2 $0, $24; ctc2 $0, $25; nop")

/* Direct BIOS B0:56 dispatch; no BIOS code is reconstructed. */
#define PSYQ_GTE_BIOS_TABLE(table)                                                                                     \
    __asm__ volatile(".set\tnoreorder\n\taddiu $10, $0, %1\n\tjalr $10\n\taddiu $9, $0, %2\n\t.set\treorder"           \
        : "=r"(table)                                                                                                  \
        : "i"(PSYQ_BIOS_TABLE_B), "i"(PSYQ_BIOS_B_GET_C0_TABLE)                                                        \
        : "$1", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25",      \
        "hi", "lo", "memory")

/* The handwritten multiply sequence contains explicit result-wait cycles. */
#define PSYQ_GTE_READ_LO_WAIT2(result) __asm__ volatile("nop; nop; mflo %0" : "=r"(result) : : "memory")
#define PSYQ_GTE_MULTIPLY_WAIT()       __asm__ volatile("nop")
/* Retain the retail sign-test timing instruction around a C mask operation. */
#define PSYQ_GTE_MASK_BRANCH_BEGIN(angle) __asm__ volatile(".set\tnoreorder\n\tbgez %0, 1f" : : "r"(angle))
#define PSYQ_GTE_MASK_BRANCH_END(angle)   __asm__ volatile("1:\n\t.set\treorder" : : "r"(angle))

/* The stack RA symbol is at0x8002bbd8, so its signed-low-half page is0x80030000.
 * A symbolic two-word address/store cannot fill the original branch delay. */
#define PSYQ_GTE_MATRIX_STACK_RA_PAGE 0x80030000U
/* The library uses its own global-RA convention for diagnostics, not O32. */
#define PSYQ_GTE_GLOBAL_PRINTF(callee, message)                                                                        \
    __asm__ volatile(".set\tnoreorder\n\tlui $4, %%hi(%1)\n\tjal %0\n\taddiu $4, $4, %%lo(%1)\n\t.set\treorder"        \
        :                                                                                                              \
        : "i"(callee), "i"(message)                                                                                    \
        : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24",       \
        "$25", "hi", "lo", "memory")
/* A C page load can fill the branch delay; retain the exact named global store. */
#define PSYQ_GTE_GLOBAL_SAVE_RA_PAGE(page, global)                                                                     \
    __asm__ volatile("sw $31, %%lo(%2)(%1)" : "=m"(global) : "r"(page), "i"(&(global)))
/* An isolated control-register read needs the original load-latency wait. */
#define PSYQ_GTE_CFC2_NOP(reg, value) __asm__ volatile("cfc2 %0, $" PSYQ_GTE_STRINGIFY(reg) "; nop" : "=r"(value))

/* The shared call/store tail begins20bytes after the short-input entry. */
#define PSYQ_GTE_VECTOR_NORMAL_SS_TAIL_OFFSET 20
/* This terminal ABI transfer saves architectural RA in a3 and branches to the
 * short-output continuation, with t0-t2 and destination a1 already live.
 * The one-shot pseudo-op accepts only the reserved RA terminator and removes
 * itself; all three input loads remain C and no instructions are discarded. */
#define PSYQ_GTE_NORMAL_SHORT_TAIL(entry, destination, x, y, z)                                                        \
    do {                                                                                                               \
        __asm__ volatile(".ifnc %0,$8\n\t.error \"GTE short tail requires t0\"\n\t.endif\n\t.ifnc %1,$9\n\t.error "    \
                         "\"GTE short tail requires t1\"\n\t.endif\n\t.ifnc %2,$10\n\t.error \"GTE short tail "        \
                         "requires t2\"\n\t.endif\n\t.ifnc %3,$5\n\t.error \"GTE short tail requires destination "     \
                         "a1\"\n\t.endif\n\t.set\tnoreorder\n\t.macro j architectural_ra\n\t.ifnc "                    \
                         "\\architectural_ra,$31\n\t.error \"GTE short tail requires architectural "                   \
                         "RA\"\n\t.endif\n\tb %4\n\taddu $7, $31, $0\n\t.purgem j\n\t.endm"                            \
            :                                                                                                          \
            : "r"(x), "r"(y), "r"(z), "r"(destination), "i"((u8*)(entry) + PSYQ_GTE_VECTOR_NORMAL_SS_TAIL_OFFSET)      \
            : "$7", "memory");                                                                                         \
        goto* psyq_cpu_return_address;                                                                                 \
    } while (0)
#define PSYQ_GTE_NORMAL_SHORT_TAIL_END() __asm__(".set\treorder")

#endif
