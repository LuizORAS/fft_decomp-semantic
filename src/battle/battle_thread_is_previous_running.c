#include "fft/thread.h"

/* The original entry uses $at address loads and fixed $t0/$t1 scratch registers. Thread 0
 * deliberately underflows to the preceding 0x400-byte record; callers must preserve the scheduler's
 * slot-order invariant. `.set noat` only suppresses the assembler warning for using $at as an
 * ordinary register. */
__asm__(".set noat");

/* Return the running flag of the slot before the current thread's (from slot 0, the record
 * before the array). */
s32 battle_thread_is_previous_running(void) {
    register s32 thread_offset __asm__("$8");
    register thread_t* threads __asm__("$9");
    register thread_t* thread __asm__("$8");

    THREAD_LOAD_GLOBAL(thread_offset, g_thread_current_id);
    __asm__("nop\naddi $8,$8,-1\nsll $8,$8,10" : "=r"(thread_offset) : "0"(thread_offset));
    THREAD_LOAD_GLOBAL(threads, g_threads);
    __asm__("nop\naddu %0,%1,%2" : "=r"(thread) : "0"(thread_offset), "r"(threads));
    return thread->is_running;
}
