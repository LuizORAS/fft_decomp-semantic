#include "fft/world.h"

/* Clear every battle slot's event status snapshot: none saved and no staged infliction pending. */
void world_unit_clear_status_staging_data(void) {
    s32 unit_index;

    for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
        g_world_unit_status_staging_data->state[unit_index] = UNIT_STATUS_STAGING_NONE;
        g_world_unit_status_staging_data->inflict_status_mode[unit_index] = UNIT_INFLICT_STATUS_MODE_NONE;
    }
}
