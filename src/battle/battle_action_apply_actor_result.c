#include "fft/battle.h"
#include "psx/types.h"

/* Apply the actor's own result (g_current_action_data, the attacker record that the formulas fill)
 * to unit_id: copy it into the unit's result record and apply it
 * (battle_action_finalize_attack_and_flag_reactions); a miss is marked CANCELLED. Returns -1 for an
 * absent unit, else the apply result. The AI simulation and
 * battle_unit_update_attack_result_animation call it. */
s32 battle_action_apply_actor_result(s32 unit_id) {
    battle_stats_t* stats;
    battle_action_data_t* action;
    s32 result;

    stats = &g_battle_unit_stats[unit_id];
    g_battle_action_target = stats;
    if (unit_id >= BATTLE_UNIT_SLOT_COUNT) {
        return -1;
    }
    action = &g_battle_unit_stats[unit_id].action;
    g_current_ability.target_id = unit_id;
    g_battle_action_target_data = action;
    if (stats->entd_slot == BATTLE_ENTD_SLOT_NONE) {
        return -1;
    }
    main_util_copy_byte_data(&g_current_action_data, action, 0x2C);
    result = battle_action_finalize_attack_and_flag_reactions(unit_id);
    if (g_battle_action_target_data->hit == 0) {
        g_battle_action_target_data->miss_type = BATTLE_ACTION_MISS_TYPE_CANCELLED;
    }
    return result;
}
