#include "fft/battle.h"
#include "psx/types.h"

/* Start the effect-message queue over and queue each target's messages
 * (battle_action_queue_unit_effect_messages). BATTLE_MESSAGE_DISPLAY starts with it. */
void battle_action_queue_target_effect_messages(void) {
    battle_unit_misc_data_t* base;
    s32 i;

    g_battle_action_post_effect_msg_counter = 0;
    base = battle_unit_get_casting_misc_data();
    for (i = 0; i < base->target_count; i++) {
        battle_action_queue_unit_effect_messages(battle_unit_get_misc_data_by_battle_id(base->target_list[i]));
    }
}
