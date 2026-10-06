#include "fft/battle.h"

/* Clear every unit slot's event staging state and exit mode (g_battle_unit_status_staging_data);
 * the event interpreter (battle_script_execute_event) calls it. */
void battle_unit_clear_status_staging_data(void) {
    s32 i;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        g_battle_unit_status_staging_data->state[i] = 0;
        g_battle_unit_status_staging_data->exit_mode[i] = 0;
    }
}
