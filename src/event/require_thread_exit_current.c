#include "fft/event_require.h"
#include "fft/thread.h"

/* Stop the current thread (thread_exit_current). */
void require_thread_exit_current(void) {
    thread_exit_current();
}
