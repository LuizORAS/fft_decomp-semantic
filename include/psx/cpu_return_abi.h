#ifndef PSX_CPU_RETURN_ABI_H
#define PSX_CPU_RETURN_ABI_H
#include "psx/types.h"
/* Reserved architectural return state, as with the BIOS selector register. */
register void* psyq_cpu_return_address __asm__("$31");
register s32 psyq_cpu_return_value __asm__("$2");
#endif
