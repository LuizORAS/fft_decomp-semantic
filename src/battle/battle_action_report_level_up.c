#include "fft/battle.h"

/* Report the unit's level up when it gained one (action_rewards.level_for_display): message
 * 0x183f for 180 frames, the level-up animation and effect with the cursor on the unit, and tune 1.
 * Returns 1 when reported. A movement ability's EXP reports this way too
 * (battle_move_start_next_post_move_event). */
s32 battle_action_report_level_up(battle_unit_misc_data_t* misc) {
    if (misc->action_rewards.level_for_display != 0) {
        u8 battle_id;
        battle_text_set_message_duration_frames(0xB4);
        battle_id = misc->battle_data->misc_unit_id;
        battle_menu_init_system_function(0xA, 0x183F, battle_id, battle_id, 1);
        battle_unit_set_level_up_animation(misc);
        battle_target_move_cursor_to_unit(misc);
        battle_effect_set_secondary_level_up(misc);
        main_sound_play_tune(1);
        return 1;
    }
    return 0;
}

/* padding */
