#include "fft/battle_gfx.h"

/* Step the queued unit graphics load (battle_gfx_step_queued_unit_graphics_load) while it reports
 * immediate progress (2), and return 0 once it has finished or 1 while it waits for a read or an
 * allocation. The event script's unit graphics request polls it (battle_script_process_pending_requests). */
s32 battle_gfx_poll_unit_graphics_load(void) {
    s32 status;

    do {
        status = battle_gfx_step_queued_unit_graphics_load();
    } while (status == 2);
    return status;
}
