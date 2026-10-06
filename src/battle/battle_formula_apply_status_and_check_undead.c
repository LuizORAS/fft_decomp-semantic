#include "fft/battle.h"
#include "psx/types.h"

/* The status part of a formula that also heals or damages: add the ability's status
 * (battle_formula_apply_status), then return 1 when that changed a status or the target is undead,
 * so the caller goes on with the HP part. Otherwise the action fails (forced failure) and returns 0:
 * Raise on a living target, or Death on a target immune to it. */
s32 battle_formula_apply_status_and_check_undead(void) {
    battle_formula_apply_status();
    if (g_battle_action_target_data->attack_type & BATTLE_ACTION_TYPE_STATUS_CHANGE) {
        return 1;
    }
    if (g_battle_action_target->status_sets.current[0] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_UNDEAD)) {
        return 1;
    }
    battle_formula_nullify_action();
    g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_FORCED_FAILURE;
    return 0;
}
