#include "fft/thread.h"

/* The original entry is hand-assembled: its loads go through $at as a base register
 * (`lui at / move at,at / lw t0,off(at)`) and use fixed $t0/$t1 scratch registers and a trapping
 * `add`, none of which GCC emits. The directive emits no instructions; it silences the assembler's
 * warning about $at without .set noat. */
__asm__(".set noat");

/* Same as thread_is_running, as a hand-assembled copy; the menu and event code call this
 * one. */
s32 world_thread_is_running_asm(s32 thread_id) {
    register s32 thread_offset __asm__("$8");
    register thread_t* threads __asm__("$9");
    register thread_t* thread __asm__("$8");

    thread_offset = thread_id << 10;
    THREAD_LOAD_GLOBAL(threads, g_threads);
    __asm__("nop\nadd %0,%1,%2" : "=r"(thread) : "0"(thread_offset), "r"(threads));
    return thread->is_running;
}
