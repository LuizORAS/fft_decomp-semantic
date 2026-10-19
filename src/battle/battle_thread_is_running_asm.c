#include "fft/thread.h"

/* The original entry uses $at address loads and fixed $t0/$t1 scratch registers. The 36-byte
 * routine is not compiler output: the plain `g_threads + (thread_id << 10)` compiles to 28 bytes
 * with $v0 as the base. */
__asm__(".set noat");

/* Same as thread_is_running, as a hand-assembled copy. */
s32 battle_thread_is_running_asm(s32 thread_id) {
    register s32 thread_offset __asm__("$8");
    register thread_t* threads __asm__("$9");
    register thread_t* thread __asm__("$8");

    thread_offset = thread_id << 10;
    THREAD_LOAD_GLOBAL(threads, g_threads);
    __asm__("nop\naddu %0,%1,%2" : "=r"(thread) : "0"(thread_offset), "r"(threads));
    return thread->is_running;
}
