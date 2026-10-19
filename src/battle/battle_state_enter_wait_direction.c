#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"

/* Enter WAIT_DIRECTION, where the unit picks the facing that ends its turn, starting from its
 * current facing (g_battle_state_wait_direction_facing). A rider first turns to its mount's facing. An AI unit
 * gets its facing from battle_ai_choose_wait_facing (cursor.facing_hint). */
void battle_state_enter_wait_direction(void) {
    battle_unit_t* mounted_unit;
    battle_unit_t* unit;
    s32 facing;

    battle_camera_disable_map_pan();
    unit = battle_unit_get_source();
    facing = unit->facing;
    g_battle_game_state = BATTLE_GAME_STATE_WAIT_DIRECTION;
    g_battle_state_wait_direction_facing = facing;
    unit->state_frame_counter = 0;
    if (unit->mount_state == BATTLE_UNIT_MOUNT_STATE_RIDER) {
        mounted_unit = battle_unit_get_by_runtime_id(unit->mount_partner_runtime_id);
        if (mounted_unit != 0) {
            battle_unit_decide_facing_direction(unit, mounted_unit->facing);
            if (unit->stats != 0) {
                battle_unit_set_tile_position(unit->stats->battle_id, unit->map_x, unit->map_y, unit->map_z,
                    (mounted_unit->facing / BATTLE_FACING_QUARTER_TURN));
            }
        }
    }
    if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) == 0) {
        unit->command_state.cursor.facing_hint = battle_ai_choose_wait_facing();
    }
}
