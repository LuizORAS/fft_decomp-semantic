#include "fft/battle.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Float climb-jump phases 0x2d-0x39 (stride 4; set by battle_move_start_float_climb_jump_step):
 * move, displace a unit standing on the destination tile (outside events), and at the
 * destination's entry edge, once down on the ground, float on to the tile centre
 * (battle_move_start_float_step). */
void battle_move_update_float_jump_to_entry_edge(battle_unit_t* unit) {
    s32 direction;

    direction = battle_move_get_direction(unit);
    battle_move_apply_unit_step_velocity(unit);
    if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
        battle_move_displace_unit_at_destination_tile(unit, direction);
    }
    if (battle_move_has_reached_destination_tile_entry_edge(direction, unit) != 0) {
        unit->velocity.vz = 0;
        unit->velocity.vx = 0;
        if (unit->world_coords.vy >= (s16)battle_gfx_calculate_world_height_from_world_coords(unit)) {
            unit->velocity.vy = 0;
            battle_move_start_float_step(unit, g_battle_move_current_tile, g_battle_move_destination_tile);
            unit->step_phase = g_battle_move_float_to_centre_phases[direction];
        }
    }
}
