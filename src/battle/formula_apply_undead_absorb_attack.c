#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Result of formula 0x0E: an undead target is healed by the amount and
 * receives none of the statuses of the first status byte; a target immune to
 * Dead is missed; anyone else takes it as HP damage.
 *
 * Status byte 1: 0x20 Dead, 0x10 Undead. */
void formula_apply_undead_absorb_attack(void) {
    battle_stats_t* unit = g_action_target;

    if (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_UNDEAD)] & STATUS_BYTE_MASK(STATUS_ID_UNDEAD)) {
        u16 amount;
        action_data_t* action = g_action_target_data;
        action_data_t* next;

        action->attack_type = ACTION_TYPE_HP_HEALING;
        amount = action->hp_damage;
        next = g_action_target_data;
        action->hp_damage = 0;
        action->hp_healing = amount;
        next->status_changes.infliction[0] = 0;
    } else if (unit->status_sets.immunity[STATUS_BYTE_INDEX(STATUS_ID_DEAD)] & STATUS_BYTE_MASK(STATUS_ID_DEAD)) {
        formula_force_attack_miss();
    } else {
        g_action_target_data->attack_type = ACTION_TYPE_HP_DAMAGE;
    }
}
