#include "fft/battle.h"
#include "psx/types.h"

/* Enter RESUME_ATTACK_PHASE at 60 fps and show the next queued effect message (system command 0x11
 * with its code, unit and value), taken from the end of the queue. Returns 1 when one was shown, 0
 * when the queue is empty. */
s32 battle_action_show_next_effect_message(void) {
    s32 idx;

    g_battle_game_state = BATTLE_GAME_STATE_RESUME_ATTACK_PHASE;
    g_battle_state_vsync_interval = 1;
    battle_unit_get_casting_misc_data();
    idx = g_battle_action_post_effect_msg_counter;
    if (idx != 0) {
        idx -= 1;
        g_battle_action_post_effect_msg_counter = idx;
        battle_menu_init_system_function(0x11, g_battle_action_post_effect_msgs[idx].code,
            g_battle_action_post_effect_msgs[idx].unit, g_battle_action_post_effect_msgs[idx].value, 1);
        g_battle_action_post_action = 0;
        return 1;
    }
    return 0;
}
