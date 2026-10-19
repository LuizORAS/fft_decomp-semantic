#include "fft/battle_action.h"
#include "fft/battle_menu.h"
#include "fft/battle_target.h"
#include "fft/battle_text.h"

/* Report the unit's job level up when it gained one (action_rewards.job_level_for_display): message
 * 0x183e for 180 frames, the level-up animation and effect with the cursor on the unit, and tune 3.
 * Returns 1 when reported. A movement ability's JP reports this way too
 * (battle_move_start_next_post_move_event). */
s32 battle_action_report_job_level_up(battle_unit_t* unit) {
    if (unit->strike.action_rewards.job_level_for_display != 0) {
        u8 battle_id;
        battle_text_set_message_duration_frames(0xB4);
        battle_id = unit->stats->battle_id;
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0x183E, battle_id, battle_id, 1);
        battle_unit_set_level_up_animation(unit);
        battle_target_move_cursor_to_unit(unit);
        battle_effect_set_secondary_level_up(unit);
        main_sound_play_tune(MAIN_SOUND_TUNE_JOB_LEVEL_UP);
        return 1;
    }
    return 0;
}
