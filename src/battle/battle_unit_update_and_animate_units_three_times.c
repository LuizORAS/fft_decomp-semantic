#include "fft/battle_unit.h"
#include "psx/types.h"

/* Run battle_unit_update_and_animate_units three times, for the queued unit graphics load
 * (battle_gfx_step_queued_unit_graphics_load). */
void battle_unit_update_and_animate_units_three_times(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        battle_unit_update_and_animate_units();
    }
}
