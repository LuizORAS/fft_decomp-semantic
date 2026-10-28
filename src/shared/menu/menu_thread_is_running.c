#include "fft/menu.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Return thread_id's running flag in BATTLE's thread table; twin of thread_is_running. */
s32 menu_thread_is_running(s32 thread_id) {
    return g_threads[thread_id].is_running;
}
