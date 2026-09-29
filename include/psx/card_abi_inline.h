#ifndef PSX_CARD_ABI_INLINE_H
#define PSX_CARD_ABI_INLINE_H

#include "psx/bios.h"
#include "psx/cpu_abi_inline.h"

/* BIOS table selection differs between the exception and PAD patch routines. */
#define PSYQ_CARD_BIOS_TABLE(table, service)                                                                           \
    __asm__ volatile(".set\tnoreorder\n\taddiu $10, $0, %1\n\tjalr $10\n\taddiu $9, $0, %2\n\t.set\treorder"           \
        : "=r"(table)                                                                                                  \
        : "i"(PSYQ_BIOS_TABLE_B), "i"(service)                                                                         \
        : "$1", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24", "$25",      \
        "hi", "lo", "memory")

/* The continuation store belongs in FlushCache's call delay, before it clobbers v0. */
#define PSYQ_CARD_FLUSH_AND_SAVE_CONTINUATION(callee, continuation, destination)                                       \
    __asm__ volatile(".set\tnoreorder\n\tlui $1, %%hi(%2)\n\tjal %1\n\tsw %3, %%lo(%2)($1)\n\t.set\treorder"           \
        : "=m"(continuation)                                                                                           \
        : "i"(callee), "i"(&(continuation)), "r"(destination)                                                          \
        : "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24",       \
        "$25", "hi", "lo", "memory")

#endif
