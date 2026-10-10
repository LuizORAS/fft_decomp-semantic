#include "fft/battle.h"

/* Stop the game flow after the casting unit's move and start the post-movement
 * steps.
 *
 * battle_move_set_target_for_mounted_unit_and_find_item builds the step flags in
 * g_battle_move_find_result (0 for a unit without battle data), and the
 * post-action display starts at the level-up report. When any step is pending the
 * map cursor and its tile glow are hidden. */
void battle_move_init_post_movement_display(void) {
    battle_stats_t* stats;

    battle_state_stop_game_flow();
    stats = battle_unit_get_casting_misc_data()->battle_data;
    if (stats != 0) {
        g_battle_move_find_result = battle_move_set_target_for_mounted_unit_and_find_item(stats);
    } else {
        g_battle_move_find_result = 0;
    }
    g_battle_action_post_action_display_phase = BATTLE_POST_ACTION_DISPLAY_LEVEL_UP;
    battle_move_start_next_post_movement_step();
    if (g_battle_move_find_result != 0) {
        battle_action_clear_at_list_active();
    }
    g_battle_state_animation_continue_check = 0;
}
