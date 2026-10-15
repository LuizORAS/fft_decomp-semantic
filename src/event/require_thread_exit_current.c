#include "fft/event_require.h"

/* Stop the current thread (battle_thread_exit_current). */
void require_thread_exit_current(void) {
    battle_thread_exit_current();
}
