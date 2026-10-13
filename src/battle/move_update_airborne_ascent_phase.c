#include "fft/battle.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Advances a jumping unit's ascent. Once its vertical velocity is no longer
 * negative, the unit takes the destination level and, when step flag 0x10 is
 * set, records the unit standing on the destination tile (id plus one). */
void move_update_airborne_ascent_phase(battle_unit_t* unit) {
    battle_unit_t* occupant;
    s32 direction;

    battle_gfx_load_trap_and_unit_frame_parts(
        unit, &unit->animation, unit->animation.frame, unit->animation.sequence_id);
    direction = move_get_direction(unit);
    if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
        move_displace_overlapping_unit(unit, direction);
    }
    if (unit->velocity.vy >= 0) {
        unit->step_phase = g_move_jump_descent_phases[direction];
        unit->map_z = unit->movement.destination_z;
        if (g_move_step_value.mount) {
            occupant = battle_unit_find_runtime_at_tile(
                unit->movement.destination_x, unit->movement.destination_y, unit->movement.destination_z);
            g_move_tile_occupant = occupant;
            if (occupant != 0) {
                unit->standing_on_battle_id_plus_one = occupant->stats->battle_id + 1;
                return;
            }
        }
        unit->standing_on_battle_id_plus_one = 0;
    }
}
