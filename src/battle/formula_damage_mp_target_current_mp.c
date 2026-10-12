#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x16: MP damage equal to the target's current MP, hit F(MA+X)%;
 * magic evade. */
void formula_damage_mp_target_current_mp(void) {
    battle_stats_t* target;
    action_data_t* action;
    u16 mp;

    if (formula_calculate_magical_evade() == 0) {
        if (formula_calculate_magic_accuracy_no_elemental() == 0) {
            target = g_action_target;
            action = g_action_target_data;
            mp = target->mp;
            action->attack_type = ACTION_TYPE_MP_DAMAGE;
            action->mp_damage = mp;
        }
    }
}
