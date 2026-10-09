#include "fft/battle.h"

void battle_unit_clear_status_staging_data(void) {
    s32 i;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        g_battle_unit_status_staging_data->state[i] = UNIT_STATUS_STAGING_NONE;
        g_battle_unit_status_staging_data->inflict_status_mode[i] = 0;
    }
}
