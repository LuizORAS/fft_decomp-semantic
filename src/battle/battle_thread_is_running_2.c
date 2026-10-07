#include "fft/battle.h"
#include "psx/types.h"

/*
 * The 36-byte target is not compiler output: it loads the g_battle_threads
 * pointer through $at and contains a redundant `move at,at`, so no C form can
 * reproduce it (the plain `g_battle_threads + (thread_id << 10)` version
 * compiles to 28 bytes with $v0 as the base). The $at pin and the two asm
 * statements are therefore required, and so is the file-scope `.set noat`:
 * it emits nothing and only silences the assembler's "used $at without
 * .set noat" warning. No assembler macro in this file needs $at.
 */
__asm__(".set noat");

/* Same as battle_thread_is_running, as a hand-assembled copy. */
s32 battle_thread_is_running_2(s32 thread_id) {
    register void* global_pointer __asm__("$1");
    register s32 thread_offset __asm__("$8");
    register u8* thread_array __asm__("$9");

    thread_offset = thread_id << 10;
    global_pointer = (void*)0x80160000;
    __asm__("move $1,$1" : "=r"(global_pointer) : "0"(global_pointer));
    /* Raw: 0x5f98 is the low half of g_battle_threads. */
    thread_array = *(u8**)((u8*)global_pointer + 0x5f98);
    __asm__("nop\naddu $8,$8,$9" : "=r"(thread_offset) : "0"(thread_offset), "r"(thread_array));
    return ((native_thread_t*)thread_offset)->is_running;
}
