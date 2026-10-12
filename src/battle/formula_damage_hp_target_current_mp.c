#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x44: damage equal to the target's current MP. */
void formula_damage_hp_target_current_mp(void) {
    u16 mp = g_action_target->mp;
    action_data_t* action = g_action_target_data;
    action->attack_type = ACTION_TYPE_HP_DAMAGE;
    action->hp_damage = mp;
}
