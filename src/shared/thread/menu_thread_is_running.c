#include "fft/menu.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Local menu entrypoint for the resident thread running flag. */
s32 menu_thread_is_running(s32 thread_id) {
    return g_native_threads[thread_id].is_running;
}
