#include "fft/battle.h"
#include "psx/types.h"

/* Result of formula 0x0E: an undead target is healed by the amount and
 * receives none of the statuses of the first status byte; a target immune to
 * Dead is missed; anyone else takes it as HP damage.
 *
 * Status byte 1: 0x20 Dead, 0x10 Undead. */
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
