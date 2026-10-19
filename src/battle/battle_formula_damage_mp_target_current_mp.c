#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x16, Mute: the magical evade check and hit chance (MA + X with both Faiths, no
 * Strengthen), then MP damage equal to the target's current MP. */
void battle_formula_damage_mp_target_current_mp(void) {
    battle_stats_t* target;
    battle_action_data_t* action;
    u16 mp;

    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_strengthen() == 0) {
            target = g_battle_action_target;
            action = g_battle_action_target_data;
            mp = target->mp;
            action->attack_type = BATTLE_ACTION_TYPE_MP_DAMAGE;
            action->mp_damage = mp;
        }
    }
}
