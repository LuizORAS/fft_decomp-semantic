#include "fft/battle_unit.h"
#include "psx/types.h"

/* Same as battle_unit_set_animation_based_on_mounted_state for the unit with this runtime id. */
void battle_unit_set_mounted_animation_by_runtime_id(u32 runtime_id) {
    battle_unit_set_animation_based_on_mounted_state(battle_unit_get_by_runtime_id(runtime_id & 0xffff));
}
