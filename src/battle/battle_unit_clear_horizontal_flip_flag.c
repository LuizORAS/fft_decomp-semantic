#include "fft/battle_unit.h"
#include "psx/types.h"

/* Stop mirroring the unit with this runtime id (script_mirror_sprite_event_instruction). */
void battle_unit_clear_horizontal_flip_flag(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    if (unit != 0) {
        unit->horizontal_flip_flag = 0;
    }
}
