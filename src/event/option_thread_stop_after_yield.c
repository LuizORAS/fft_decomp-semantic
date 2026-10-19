#include "fft/event_option.h"
#include "fft/thread.h"

/* Yield once, then exit the current thread. Nothing references it (QUIRKS.md). */
void option_thread_stop_after_yield(void) {
    thread_yield();
    thread_exit_current();
}
