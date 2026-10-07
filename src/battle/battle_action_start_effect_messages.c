#include "fft/battle.h"

/* Queue the casting unit's own effect messages after its targets', drop the whole queue when the
 * Effect Messages option is off, and show the first one (battle_action_show_next_effect_message);
 * with none, close the message window (system command 0xA). BATTLE_MESSAGE_DISPLAY calls it. */
void battle_action_start_effect_messages(void) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_casting_misc_data();
    battle_action_queue_unit_effect_messages(unit);
    if (g_main_game_options.fields.effect_messages != GAME_OPTION_ON) {
        g_battle_action_post_effect_msg_counter = 0;
    }
    if (battle_action_show_next_effect_message() == 0) {
        battle_menu_init_system_function(0xA, 0, unit->battle_data->misc_unit_id, 0, 0);
    }
}
