#include "fft/event_equip.h"

/* Ask thread `thread_id` to stop: set its third parameter to 1 (battle_thread_set_parameters), the
 * flag the screen's threads poll before they exit. */
void equip_thread_request_stop(s32 id) {
    battle_thread_set_parameters(id, 0, 0, 1);
}
