#include "fft/battle_action.h"
#include "psx/types.h"

/* Empty the post-effect message queue, then queue the messages of every target of
 * the casting unit. */
void battle_action_build_targets_post_action_message(void) {
    battle_unit_misc_data_t* base;
    s32 i;

    g_battle_action_post_effect_msg_counter = 0;
    base = battle_unit_get_casting_misc_data();
    for (i = 0; i < base->strike.target_count; i++) {
        battle_action_queue_post_effect_messages_for_unit(
            battle_unit_get_misc_data_by_battle_id(base->strike.target_list[i]));
    }
}
