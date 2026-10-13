#include "fft/event_option.h"
#include "fft/thread.h"

/* Park this event thread while allowing the scheduler to continue. */
void option_thread_wait_forever(void) {
    for (;;) {
        thread_yield();
    }
}
