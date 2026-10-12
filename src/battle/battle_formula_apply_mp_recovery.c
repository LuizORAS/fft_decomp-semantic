#include "fft/battle_action.h"
#include "psx/types.h"

/* MP drain step: the stored HP damage becomes MP damage on the target; when it
 * lands, the attacker recovers the same amount of MP. */
void battle_formula_apply_mp_recovery(void) {
    u16 mp_amount;
    action_data_t* action;
    action_data_t* action_after;

    action = g_action_target_data;
    mp_amount = action->hp_damage;
    action->hp_damage = 0;
    action->attack_type = ACTION_TYPE_MP_DAMAGE;
    action->mp_damage = mp_amount;
    action_finalize_target_current_action();
    action_after = g_action_target_data;
    if (action_after->hit != 0) {
        action_data_t* reaction = g_action_attacker_data;
        reaction->mp_healing = action_after->mp_damage;
        reaction->attack_type = ACTION_TYPE_MP_HEALING;
        g_action_attacker_data->hit = 1;
    }
}
