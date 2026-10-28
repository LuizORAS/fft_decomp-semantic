#include "fft/main.h"
#include "fft/world.h"
#include "psx/types.h"

/* Poll the hardware card event until it reports a result. A poll that
 * finds nothing while bit 0 of _card_status is set ends the wait with
 * CARD_IO_RESULT_TIMEOUT. */
s32 world_card_wait_for_hardware_event(void) {
    s32 busy;
    s32 result;

    while (1) {
        busy = _card_status(g_world_card_selected_slot) & 1;
        result = card_io_poll_hardware_event_result();
        if (result >= CARD_IO_RESULT_COMPLETE) {
            break;
        }
        result = CARD_IO_RESULT_TIMEOUT;
        if (busy != 0) {
            break;
        }
    }
    return result;
}
