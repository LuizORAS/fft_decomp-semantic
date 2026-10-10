#include "fft/battle.h"
#include "psx/types.h"

/* Target statuses on the evades: Don't Act, Sleep, Stop, Confusion, Charging
 * or Performing clear all four; Defending halves the base hit, which doubles
 * them. */
void battle_formula_apply_evasion_changes_due_to_statuses(void) {
    battle_stats_t* target;
    u8 flag;
    u8* base_hit;

    target = g_battle_action_target;
    flag = (target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SLEEP)]
               & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SLEEP) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DONT_ACT)))
        != 0;
    if (target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_STOP)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_STOP)) {
        flag = 1;
    }
    if (target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CONFUSION)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION)) {
        flag = 1;
    }
    if (target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CHARGING)]
        & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CHARGING) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PERFORMING))) {
        flag = 1;
    }
    if (flag) {
        main_util_clear_byte_data(&g_battle_action_current_ability.accessory_evade, 4);
    }
    if (g_battle_action_target->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEFENDING)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEFENDING)) {
        base_hit = &g_battle_action_current_ability.base_hit;
        *base_hit >>= 1;
    }
}
