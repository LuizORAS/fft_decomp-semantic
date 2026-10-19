#include "fft/battle_unit.h"

/* Set BATTLE_MOTION_FLAG_SUPPRESS_PALETTE_UPDATE on the unit with this runtime id: the status and tile tints
 * leave its palette alone (battle_script_set_units_palette_update_suppression). */
void battle_unit_set_palette_update_suppression(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    if (unit != 0) {
        unit->movement.suppress_palette_update = 1;
    }
}
