#include "fft/battle.h"

/* Report a job level up recorded in the unit's rewards: message 0x183e for three
 * seconds, the level-up animation and effect, the cursor on the unit and the job
 * level-up tune. Returns 1 after reporting one, otherwise 0. */
s32 battle_action_report_job_level_up(battle_unit_misc_data_t* misc) {
    if (misc->strike.action_rewards.job_level_for_display != 0) {
        u8 battle_id;
        battle_text_set_message_duration_frames(0xB4);
        battle_id = misc->battle_data->misc_unit_id;
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0x183E, battle_id, battle_id, 1);
        battle_unit_set_level_up_animation(misc);
        battle_target_move_cursor_to_unit(misc);
        battle_effect_set_secondary_level_up(misc);
        main_sound_play_tune(MAIN_SOUND_TUNE_JOB_LEVEL_UP);
        return 1;
    }
    return 0;
}
