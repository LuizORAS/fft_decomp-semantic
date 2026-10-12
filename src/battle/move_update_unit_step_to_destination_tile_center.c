#include "fft/battle.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Step handler of the WALK_TO_CENTRE and CLIMB_TO_CENTRE stages (and of the
 * knockback step): advance the unit one frame toward the centre of its
 * destination tile.
 *
 * Outside events it pushes aside a unit on the destination tile. At the centre
 * the horizontal motion stops; once the unit is also down at the ground height
 * there, it takes the destination tile as its map position and the step ends. */
void move_update_unit_step_to_destination_tile_center(battle_unit_misc_data_t* unit) {
    s32 direction;
    s16 screen_z;

    direction = move_get_direction(unit);
    move_apply_unit_step_velocity(unit);
    if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
        move_displace_unit_at_destination_tile(unit, direction);
    }
    if (move_has_reached_destination_tile_center(direction, unit) != 0) {
        move_snap_axis_to_destination_tile_center(direction, unit);
        unit->velocity.vz = 0;
        unit->velocity.vx = 0;
        screen_z = battle_gfx_calculate_screen_z_from_misc_screen_data(unit);
        g_move_target_screen_z = screen_z;
        if (unit->screen.vy >= screen_z) {
            unit->map_x = unit->movement.destination_x;
            unit->map_y = unit->movement.destination_y;
            unit->velocity.vy = 0;
            unit->real.vy = g_move_target_screen_z << 12;
            unit->screen.vy = unit->real.vy / ONE;
            unit->step_phase = MOVE_STEP_PHASE_NONE;
            unit->shadow_dirty |= 1;
        }
    }
}
