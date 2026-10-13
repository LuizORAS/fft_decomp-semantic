#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_move.h"

/* Stop the game flow after the casting unit's move and start the post-movement
 * steps.
 *
 * move_set_target_for_mounted_unit_and_find_item builds the step flags in
 * g_move_find_result (0 for a unit without battle data), and the
 * post-action display starts at the level-up report. When any step is pending the
 * map cursor and its tile glow are hidden. */
void move_init_post_movement_display(void) {
    battle_stats_t* stats;

    camera_disable_map_pan();
    stats = battle_unit_get_casting()->battle_data;
    if (stats != 0) {
        g_move_find_result = move_set_target_for_mounted_unit_and_find_item(stats);
    } else {
        g_move_find_result = 0;
    }
    g_action_post_action_display_phase = BATTLE_POST_ACTION_DISPLAY_LEVEL_UP;
    move_start_next_post_movement_step();
    if (g_move_find_result != 0) {
        action_clear_at_list_active();
    }
    g_battle_state_animation_continue_check = 0;
}
