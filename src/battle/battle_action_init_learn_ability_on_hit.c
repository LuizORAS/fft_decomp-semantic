#include "fft/battle.h"
#include "psx/types.h"

/* Enter LEARN_ABILITY_ON_HIT at 60 fps and show the post-action report of the current phase
 * (g_battle_action_post_action_display_phase): 0 a level up, 1 a job level up, 2 a unit that learns
 * the ability it was hit with (battle_ability_find_learn_on_hit_unit), with the cursor on it. Without
 * one the phase advances. A message lasts 30 frames unless the report sets its own. */
void battle_action_init_learn_ability_on_hit(void) {
    battle_unit_misc_data_t* unit;
    battle_unit_misc_data_t* learner;
    u16 ability_id;

    g_battle_game_state = BATTLE_GAME_STATE_LEARN_ABILITY_ON_HIT;
    g_main_system_vsync_interval = 1;
    unit = battle_unit_get_source_misc_data();
    battle_menu_init_system_function(
        BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0, unit->battle_data->misc_unit_id, 0, 0);
    battle_text_set_message_duration_frames(0x1E);
    switch (g_battle_action_post_action_display_phase) {
    case BATTLE_POST_ACTION_DISPLAY_LEVEL_UP:
        battle_action_report_level_up(unit);
        break;
    case BATTLE_POST_ACTION_DISPLAY_JOB_LEVEL_UP:
        battle_action_report_job_level_up(unit);
        break;
    case BATTLE_POST_ACTION_DISPLAY_LEARN_ON_HIT:
        g_battle_action_learn_on_hit_unit_id
            = battle_ability_find_learn_on_hit_unit(unit->battle_data->misc_unit_id, &ability_id);
        if (g_battle_action_learn_on_hit_unit_id != -1) {
            learner = battle_unit_get_misc_data_by_battle_id(g_battle_action_learn_on_hit_unit_id);
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_ABILITY_LEARNED, ability_id,
                unit->battle_data->misc_unit_id, 0,
                learner->battle_data->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
            battle_target_move_cursor_to_unit(learner);
            battle_target_show_cursor();
        } else {
            g_battle_action_post_action_display_phase++;
        }
        break;
    }
    battle_target_show_cursor_unit_panel();
}
