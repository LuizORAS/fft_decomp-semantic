#include "fft/battle.h"

/* Per-frame float height and status bubble of one unit: the height offset moves
 * (battle_unit_increment_or_decrement_height_mod) except in the float move's jump phases (0x2d, 0x31,
 * 0x35, 0x39), then an active bubble advances (battle_gfx_determine_status_bubble_parameters). */
void battle_gfx_update_float_height_and_status_bubble(battle_unit_misc_data_t* unit) {
    u8 tile_offset;

    tile_offset = unit->step_phase;
    if (tile_offset != 0x2D && tile_offset != 0x31 && tile_offset != 0x39 && tile_offset != 0x35) {
        battle_unit_increment_or_decrement_height_mod(unit);
    }
    if (unit->status_bubble_active != 0) {
        battle_gfx_determine_status_bubble_parameters(unit);
    }
}
