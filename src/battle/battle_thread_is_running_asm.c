#include "fft/thread.h"
#include "psx/types.h"

/*
 * The hand-assembled twin of battle_thread_is_running; the menu and event code
 * call this one. Reads g_native_threads[thread_id].is_running (thread records
 * are 0x400 bytes; the 32-bit value sits at +0x48).
 *
 * The 36-byte target is not compiler output: it loads the g_native_threads
 * pointer through $at and contains a redundant `move at,at`, so no C form can
 * reproduce it (the plain `g_native_threads + (thread_id << 10)` version
 * compiles to 28 bytes with $v0 as the base). The $at pin and the two asm
 * statements are therefore required, and so is the file-scope `.set noat`:
 * it emits nothing and only silences the assembler's "used $at without
 * .set noat" warning. No assembler macro in this file needs $at.
 */
__asm__(".set noat");

s32 battle_thread_is_running_asm(s32 thread_id) {
    register void* global_pointer __asm__("$1");
    register s32 thread_offset __asm__("$8");
    register u8* thread_array __asm__("$9");

    thread_offset = thread_id << 10;
    global_pointer = (void*)0x80160000;
    __asm__("move $1,$1" : "=r"(global_pointer) : "0"(global_pointer));
    thread_array = *(u8**)((u8*)global_pointer + 0x5f98);
    __asm__("nop\naddu $8,$8,$9" : "=r"(thread_offset) : "0"(thread_offset), "r"(thread_array));
    return ((thread_t*)thread_offset)->is_running;
}
