#include "fft/battle.h"

/* Compute the post-move events of the acting unit into g_battle_move_post_move_events and start the
 * first (battle_move_start_next_post_move_event; with none pending it moves on to the after-command
 * state); the map cursor hides while events are pending. battle_unit_copy_rider_data_to_mount calls
 * it at the end of a move. */
void battle_move_start_post_move_events(void) {
    battle_stats_t* stats;

    battle_state_disable_camera_pan();
    stats = battle_unit_get_casting_misc_data()->battle_data;
    if (stats != 0) {
        g_battle_move_post_move_events = battle_move_get_post_move_events(stats);
    } else {
        g_battle_move_post_move_events = 0;
    }
    g_battle_action_post_action_display_phase = 0;
    battle_move_start_next_post_move_event();
    if (g_battle_move_post_move_events != 0) {
        battle_target_hide_cursor();
    }
    g_battle_state_animation_continue_check = 0;
}
