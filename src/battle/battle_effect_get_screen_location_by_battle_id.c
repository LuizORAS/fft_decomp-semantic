#include "fft/battle_effect.h"
#include "fft/battle_unit.h"

/* Resolve a battle ID before calculating its effect-space screen location. */
void battle_effect_get_screen_location_by_battle_id(s32 adjust_for_tile_height, u32 battle_id,
    battle_effect_secondary_init_t* effect_coordinates, SVECTOR* destination) {
    battle_effect_resolve_target_render_coords(
        adjust_for_tile_height, battle_unit_get_misc_id_by_battle_id(battle_id), effect_coordinates, destination);
}
