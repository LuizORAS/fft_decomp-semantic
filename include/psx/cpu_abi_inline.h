#ifndef PSX_CPU_ABI_INLINE_H
#define PSX_CPU_ABI_INLINE_H

/* Handwritten CPU transport and arithmetic boundaries; no register reservations.
 * Trapping ADD/SUB differ from the compiler's usual ADDU/SUBU lowering. */
#define PSYQ_CPU_STRINGIFY_VALUE(value) #value
#define PSYQ_CPU_STRINGIFY(value)       PSYQ_CPU_STRINGIFY_VALUE(value)

#define PSYQ_CPU_TRAP_ADD(result, a, b) __asm__ volatile("add %0, %1, %2" : "=r"(result) : "r"(a), "r"(b))

#define PSYQ_CPU_TRAP_SUB(result, a, b) __asm__ volatile("sub %0, %1, %2" : "=r"(result) : "r"(a), "r"(b))

#define PSYQ_CPU_TRAP_ADDI(result, a, imm) __asm__ volatile("addi %0, %1, " #imm : "=r"(result) : "r"(a))

#define PSYQ_CPU_SIGNED_CONSTANT(result, imm) __asm__("addiu %0, $0, " #imm : "=r"(result))

#define PSYQ_CPU_GLOBAL_SAVE_RA(global) __asm__ volatile("sw $31, %0" : "=m"(global))

#define PSYQ_CPU_GLOBAL_RESTORE_RA(global)                                                                             \
    __asm__ volatile("lui $31, %%hi(%0); lw $31, %%lo(%0)($31); nop" : : "i"(&(global)) : "memory")

#define PSYQ_CPU_GLOBAL_CALL(callee)                                                                                   \
    __asm__ volatile("jal %0"                                                                                          \
        :                                                                                                              \
        : "i"(callee)                                                                                                  \
        : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24",       \
        "$25", "hi", "lo", "memory")

#define PSYQ_CPU_STATUS_READ(status) __asm__ volatile("mfc0 %0, $12" : "=r"(status))

#define PSYQ_CPU_STATUS_WRITE(status) __asm__ volatile("mtc0 %0, $12; nop" : : "r"(status))

#define PSYQ_CPU_ADDRESS_HIGH(pointer, symbol)                                                                         \
    __asm__ volatile("lui %0, %%hi(%1)" : "=r"(pointer) : "i"(symbol) : "memory")

#define PSYQ_CPU_ADDRESS_LOW(pointer, symbol)                                                                          \
    __asm__ volatile("addiu %0, %1, %%lo(%2)" : "=r"(pointer) : "0"(pointer), "i"(symbol) : "memory")

#define PSYQ_CPU_SHARED_DELAY_BEGIN() __asm__ volatile(".set\tnoreorder")

#define PSYQ_CPU_SHARED_DELAY_END() __asm__ volatile(".set\treorder")

#define PSYQ_CPU_LOAD_WAIT() __asm__ volatile("nop")

/* No O32 argument area: the caller owns the handwritten 16-byte dispatch frame. */
#define PSYQ_CRT_DISPATCH_CALL(handler)                                                                                \
    __asm__ volatile(".set\tnoreorder\n\t.ifnc %0,$8\n\t.error \"CRT callback requires t0\"\n\t.endif\n\tjalr %0"      \
        :                                                                                                              \
        : "r"(handler)                                                                                                 \
        : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25",      \
        "hi", "lo", "memory")

#endif
