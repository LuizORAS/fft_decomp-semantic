#include "fft/battle.h"
#include "psx/types.h"

void battle_move_apply_knockback(void) {
    u8* flags;
    battle_stats_t* unit;
    battle_action_data_t* action;
    s32 amount;
    s32 diff;

    battle_action_clear_data();
    flags = &g_battle_action_current_ability.knockback_flags;
    *flags &= BATTLE_KNOCKBACK_KIND_MASK;
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    unit = g_battle_action_target;
    amount = 0;
    if (!(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_CRYSTAL)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CRYSTAL))
        && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DEAD)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DEAD))
        && !(unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_PETRIFY)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_PETRIFY))) {
        if (*flags == BATTLE_KNOCKBACK_FALL_DAMAGE) {
            diff = (g_battle_action_current_ability.knockback_fall_height >> 1) - unit->jump;
            if (diff > 0) {
                amount = (diff * unit->max_hp) / 10;
                if (amount >= 0x3E8) {
                    amount = 0x3E7;
                }
            }
        }
        action = g_battle_action_target_data;
        action->hp_damage = amount;
        if (amount != 0) {
            action->attack_type |= BATTLE_ACTION_TYPE_HP_DAMAGE;
        }
    }
}
