#include "fft/battle_action.h"
#include "psx/types.h"

/* Start the effect-message queue over and queue each target's messages
 * (battle_action_queue_unit_effect_messages). BATTLE_MESSAGE_DISPLAY starts with it. */
void battle_action_queue_target_effect_messages(void) {
    battle_unit_t* base;
    s32 i;

    g_battle_action_post_effect_msg_counter = 0;
    base = battle_unit_get_casting();
    for (i = 0; i < base->strike.target_count; i++) {
        battle_action_queue_unit_effect_messages(battle_unit_get_by_battle_id(base->strike.target_list[i]));
    }
}
