#include "fft/battle_action.h"
#include "psx/types.h"
/* Record reaction_id when the ability cost MP and the target passes its reaction
 * roll; the MP cost is the reaction's amount. */
void action_check_face_up_and_absorb_used_mp_usability(s16 reaction_id) {
    u8* used_mp = &g_action_current_ability.mp_cost;
    action_data_t* action;
    if (*used_mp != 0) {
        if (action_calculate_chance_to_react(g_action_target) == 0) {
            action = g_action_target_data;
            action->reaction_id = reaction_id;
            action->last_received_attack = *used_mp;
        }
    }
}
