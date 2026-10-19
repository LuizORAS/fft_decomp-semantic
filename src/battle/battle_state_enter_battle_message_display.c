#include "fft/battle.h"
#include "psx/types.h"

/* Enter BATTLE_MESSAGE_DISPLAY at 60 fps after the strike: build the targets' result messages
 * and animations, then show the ability's message for 60 frames. Formulas 6 and 9 play tune 2
 * with messages 0x1838 and 0x1828; otherwise the first target hit gives the message from the
 * ability animation table (text_display), where 6 also plays tune 2 and 6 and 0x2b name both
 * units. */
void battle_state_enter_battle_message_display(void) {
    battle_unit_t* unit;
    battle_unit_t* target;
    battle_ability_animation_data_t* animation;
    s32 message;
    s32 i;

    g_battle_game_state = BATTLE_GAME_STATE_BATTLE_MESSAGE_DISPLAY;
    g_main_system_vsync_interval = 1;
    g_battle_action_post_action = 0;
    unit = battle_unit_get_casting();
    battle_action_queue_target_effect_messages();
    battle_unit_update_attack_result_animation(unit);
    unit->item_display_active = 0;
    battle_text_set_message_duration_frames(0x3c);
    switch (unit->strike.post_action_message) {
    case BATTLE_POST_ACTION_MESSAGE_POACHED:
        main_sound_play_tune(MAIN_SOUND_TUNE_ITEM_FOUND);
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0x1838, unit->stats->battle_id, 0, 1);
        return;
    case BATTLE_POST_ACTION_MESSAGE_TRAINED:
        main_sound_play_tune(MAIN_SOUND_TUNE_ITEM_FOUND);
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0x1828, unit->stats->battle_id, 0, 1);
        return;
    }

    message = 0;
    for (i = 0; i < unit->strike.target_count; i++) {
        target = battle_unit_get_by_battle_id(unit->strike.target_list[i]);
        if (target != 0 && target->stats != 0 && target->stats->action.hit != 0) {
            battle_text_set_message_duration_frames(0x3c);
            /* Taking the row first keeps this loop above loop.c's size limit
             * for hoisting the constant 6 into a saved register. */
            animation = &g_battle_ability_animation_data[unit->sp2_ability_id];
            message = animation->text_display;
            if (message == 0) {
                break;
            }
            if (message == 6) {
                main_sound_play_tune(MAIN_SOUND_TUNE_ITEM_FOUND);
            }
            break;
        }
    }

    if (message == 6 || message == 0x2b) {
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, message | 0x1800,
            unit->stats->battle_id, target->stats->battle_id, 1);
    } else if (message != 0) {
        battle_menu_init_system_function(
            BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, message | 0x1800, unit->stats->battle_id, 0, 1);
    } else {
        battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0, unit->stats->battle_id, 0, 0);
    }
}
