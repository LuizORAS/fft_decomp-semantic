#include "fft/battle.h"
#include "psx/types.h"

void battle_move_advance_display_unit_step(battle_unit_misc_data_t* unit) {
    s32 direction;

    direction = battle_move_get_direction(unit);
    switch (g_battle_move_step_value.bits.climb) {
    case 0:
        battle_move_start_unit_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->centre_tile_offset = g_battle_move_step_centre_offsets[direction];
        break;
    case 1:
        battle_move_start_unit_step_at_climb_speed(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
        unit->centre_tile_offset = g_battle_move_climb_step_centre_offsets[direction];
        break;
    }
}
