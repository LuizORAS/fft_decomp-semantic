#ifndef PSX_ABI_INLINE_H
#define PSX_ABI_INLINE_H

#include "psx/bios.h"
#include "psx/types.h"

/* BIOS tail veneers must leave the selector live in t1 when control transfers.
 * A local unused selector assignment is removed or cannot fill the delay slot.
 * Only veneer sources include this reserved-register view; it allocates no RAM. */
register s32 psyq_bios_service_register __asm__("$9");

#define PSYQ_BIOS_TAIL_CALL(vector, service)                                                                           \
    do {                                                                                                               \
        register s32 zero __asm__("$0");                                                                               \
        register s32 dispatcher __asm__("$10");                                                                        \
        __asm__("" : "=r"(zero)); /* Physical zero produces the retail signed-immediate loads. */                      \
        dispatcher = zero + (vector);                                                                                  \
        psyq_bios_service_register = zero + (service);                                                                 \
        goto*(void*)dispatcher; /* BIOS returns directly to the original caller. */                                    \
    } while (0)

/* SYSCALL is an ABI hardware primitive; C emits setup and return instructions. */
#define PSYQ_BIOS_SYSCALL(selector)                                                                                    \
    do {                                                                                                               \
        register s32 zero __asm__("$0");                                                                               \
        register s32 operation __asm__("$4");                                                                          \
        __asm__("" : "=r"(zero)); /* Preserve original ADDIU rather than ORI. */                                       \
        operation = zero + (selector);                                                                                 \
        __asm__ volatile("syscall" : : "r"(operation) : "memory");                                                     \
    } while (0)

#endif
