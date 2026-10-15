#include "fft/battle.h"
#include "psx/types.h"

/* The retail routine loads both globals through $at, including redundant
 * `move at,at` instructions, and uses fixed $t0/$t1 scratch registers. GCC
 * does not generate that sequence, so the register constraints, raw global
 * offsets, and inline instructions are required to preserve the target's
 * scheduling; named-global accesses break the byte-exact match. Thread 0 deliberately
 * underflows to the preceding 0x400-byte record; callers must preserve the
 * scheduler's slot-order invariant. `.set noat` only suppresses the assembler
 * warning for using $at as an ordinary register. */
__asm__(".set noat");

/* Return the running flag of the slot before the current thread's (from slot 0, the record
 * before the array). */
s32 battle_thread_is_previous_running(void) {
    register s32 offset __asm__("$8");
    register void* global_pointer __asm__("$1") = (void*)0x80170000;
    register u8* thread_array __asm__("$9");

    __asm__("move $1,$1" : "=r"(global_pointer) : "0"(global_pointer));
    /* Raw: 0x4038/0x5f98 are the low halves of g_battle_thread_current_id and g_battle_threads. */
    offset = *(s32*)((u8*)global_pointer + 0x4038);
    __asm__("nop\naddi $8,$8,-1\nsll $8,$8,10" : "=r"(offset) : "0"(offset));
    global_pointer = (void*)0x80160000;
    __asm__("move $1,$1" : "=r"(global_pointer) : "0"(global_pointer));
    thread_array = *(u8**)((u8*)global_pointer + 0x5f98);
    __asm__("nop\naddu $8,$8,$9" : "=r"(offset) : "0"(offset), "r"(thread_array));
    return ((native_thread_t*)offset)->is_running;
}
