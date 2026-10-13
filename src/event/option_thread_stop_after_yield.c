#include "fft/event_option.h"

/* Yield once, then exit the current thread. Nothing references it (QUIRKS.md). */
void option_thread_stop_after_yield(void) {
    battle_thread_yield();
    battle_thread_exit_current();
}
