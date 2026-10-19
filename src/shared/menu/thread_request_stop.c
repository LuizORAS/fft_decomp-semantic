#include "fft/menu.h"
#include "fft/thread.h"

/* Ask thread `thread_id` to stop: set its third parameter to 1 (thread_set_parameters), the
 * flag the screen's threads poll before they exit. */
void thread_request_stop(s32 thread_id) {
    thread_set_parameters(thread_id, 0, 0, 1);
}
