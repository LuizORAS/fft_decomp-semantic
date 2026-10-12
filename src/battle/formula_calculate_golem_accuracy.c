#include "fft/battle_action.h"
#include "psx/types.h"

/* Golem's hit chance: (MA with Magic Attack Up + X) * caster Faith / 100, the
 * Faith status counting as 100 and Innocent as 0. No zodiac compatibility and
 * no target Faith. Returns 1 on a miss. */
s32 formula_calculate_golem_accuracy(void) {
    action_data_t* action;

    formula_store_ma_and_x();
    formula_apply_magic_attack_up();
    formula_store_xa_plus_ya_status_damage();
    if ((g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FAITH)] & STATUS_BYTE_MASK(STATUS_ID_FAITH))
        != 0) {
        g_action_current_ability.attacker_faith = 0x64;
    }
    if ((g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_INNOCENT)]
            & STATUS_BYTE_MASK(STATUS_ID_INNOCENT))
        != 0) {
        g_action_current_ability.attacker_faith = 0;
    }
    action = g_action_target_data;
    /* The target reads hp_damage signed (lh). */
    action->hp_damage = action->hp_damage * g_action_current_ability.attacker_faith / 100;
    formula_use_hp_damage_as_action_hit_percent();
    return g_action_target_data->hit == 0;
}
