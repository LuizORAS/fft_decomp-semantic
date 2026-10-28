#include "fft/battle_unit.h"
#include "psx/types.h"

/* Same as battle_unit_update_display for the unit with this runtime id. */
void battle_unit_update_display_by_runtime_id(s32 runtime_id) {
    battle_unit_update_display(battle_unit_get_by_runtime_id(runtime_id));
}
