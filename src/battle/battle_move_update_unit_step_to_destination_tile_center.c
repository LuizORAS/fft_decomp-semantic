#include "fft/battle.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Walk phases 2-16 (even; edge to center), also run by the knockback step: move, displace a unit
 * standing on the destination tile (outside events), and at the center snap to it; once the unit
 * is down on the ground, take the destination tile and end the step (phase 0). The float copy,
 * battle_move_update_float_step_to_destination_tile_center (float phases 0x2c-0x38, stride 4), is
 * this text with the displacement bound to battle_move_displace_overlapping_unit, so it pushes aside
 * a unit the floating unit overlaps. */
void battle_move_update_unit_step_to_destination_tile_center(battle_unit_t* unit) {
    s32 direction;
    s16 screen_z;

    direction = battle_move_get_direction(unit);
    battle_move_apply_unit_step_velocity(unit);
    if (g_battle_game_state != BATTLE_GAME_STATE_EVENT) {
        battle_move_displace_unit_at_destination_tile(unit, direction);
    }
    if (battle_move_has_reached_destination_tile_center(direction, unit) != 0) {
        battle_move_snap_axis_to_destination_tile_center(direction, unit);
        unit->velocity.vz = 0;
        unit->velocity.vx = 0;
        screen_z = battle_gfx_calculate_world_height_from_world_coords(unit);
        g_battle_move_target_screen_z = screen_z;
        if (unit->world_coords.vy >= screen_z) {
            unit->map_x = unit->movement.destination_x;
            unit->map_y = unit->movement.destination_y;
            unit->velocity.vy = 0;
            unit->real.vy = g_battle_move_target_screen_z * ONE;
            unit->world_coords.vy = unit->real.vy / ONE;
            unit->step_phase = BATTLE_MOVE_STEP_PHASE_NONE;
            unit->shadow_dirty |= 1;
        }
    }
}
