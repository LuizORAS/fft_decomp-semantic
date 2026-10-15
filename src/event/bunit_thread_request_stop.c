#include "fft/event_bunit.h"
#include "psx/types.h"

/* Ask thread `thread_id` to stop: set its third parameter to 1 (battle_thread_set_parameters), the
 * flag the screen's threads poll before they exit. */
void bunit_thread_request_stop(s32 thread_id) {
    battle_thread_set_parameters(thread_id, 0, 0, 1);
}
