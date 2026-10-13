#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Target statuses on the evades: Don't Act, Sleep, Stop, Confusion, Charging
 * or Performing clear all four; Defending halves the base hit, which doubles
 * them. */
void formula_apply_evasion_changes_due_to_statuses(void) {
    unit_stats_t* target;
    u8 flag;
    u8* base_hit;

    target = g_action_target;
    flag = (target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_SLEEP)]
               & (STATUS_BYTE_MASK(STATUS_ID_SLEEP) | STATUS_BYTE_MASK(STATUS_ID_DONT_ACT)))
        != 0;
    if (target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_STOP)] & STATUS_BYTE_MASK(STATUS_ID_STOP)) {
        flag = 1;
    }
    if (target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CONFUSION)] & STATUS_BYTE_MASK(STATUS_ID_CONFUSION)) {
        flag = 1;
    }
    if (target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CHARGING)]
        & (STATUS_BYTE_MASK(STATUS_ID_CHARGING) | STATUS_BYTE_MASK(STATUS_ID_PERFORMING))) {
        flag = 1;
    }
    if (flag) {
        main_util_clear_byte_data(&g_action_current_ability.accessory_evade, 4);
    }
    if (g_action_target->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEFENDING)]
        & STATUS_BYTE_MASK(STATUS_ID_DEFENDING)) {
        base_hit = &g_action_current_ability.base_hit;
        *base_hit >>= 1;
    }
}
