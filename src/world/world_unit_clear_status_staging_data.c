#include "fft/world.h"

void world_unit_clear_status_staging_data(void) {
    s32 unit_index;

    unit_index = 0;
    do {
        g_world_unit_status_staging_data->state[unit_index] = UNIT_STATUS_STAGING_NONE;
        g_world_unit_status_staging_data->inflict_status_mode[unit_index] = 0;
        unit_index++;
    } while (unit_index < BATTLE_UNIT_SLOT_COUNT);
}
