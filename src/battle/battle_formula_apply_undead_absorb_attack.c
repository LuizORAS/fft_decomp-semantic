#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* The HP part of Death (formula 0x0e): an undead target heals the HP damage instead and gets no
 * status from the first byte (Dead); a target immune to Dead is missed; any other target takes the
 * HP damage. */
void battle_formula_apply_undead_absorb_attack(void) {
    battle_stats_t* unit = g_battle_action_target;

    if (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_UNDEAD)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD)) {
        u16 amount;
        battle_action_data_t* action = g_battle_action_target_data;
        battle_action_data_t* next;

        action->attack_type = BATTLE_ACTION_TYPE_HP_HEALING;
        amount = action->hp_damage;
        next = g_battle_action_target_data;
        action->hp_damage = 0;
        action->hp_healing = amount;
        next->status_changes.infliction[0] = 0;
    } else if (unit->status_sets.immunity[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD)) {
        battle_formula_force_attack_miss();
    } else {
        g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
    }
}
