#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"

/* Enter the wait-direction state for the source unit, starting from its current
 * facing.
 *
 * A rider first takes its mount's facing. A unit the player does not control gets
 * its facing from ai_choose_wait_facing as the cursor's facing hint. */
void action_choose_facing_for_wait(void) {
    battle_unit_t* mounted_unit;
    battle_unit_t* unit;
    s32 facing;

    camera_disable_map_pan();
    unit = battle_unit_get_source();
    facing = unit->facing;
    g_battle_game_state = BATTLE_GAME_STATE_WAIT_DIRECTION;
    g_battle_state_wait_direction_facing = facing;
    unit->state_frame_counter = 0;
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        mounted_unit = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        if (mounted_unit != 0) {
            battle_unit_decide_facing_direction(unit, mounted_unit->facing);
            if (unit->battle_data != 0) {
                battle_unit_set_tile_position(unit->battle_data->battle_id, unit->map_x, unit->map_y, unit->map_z,
                    (mounted_unit->facing / BATTLE_FACING_QUARTER_TURN));
            }
        }
    }
    if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) == 0) {
        unit->command_state.cursor.facing_hint = ai_choose_wait_facing();
    }
}
