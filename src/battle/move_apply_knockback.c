#include "fft/battle_action.h"
#include "fft/battle_move.h"
#include "psx/types.h"

/* Resolve a pending knockback as the target's own action. A grounded target
 * that is not a crystal, dead or petrified takes (fall height / 2 - Jump) *
 * MaxHP / 10 fall damage when that is positive, at most 999; a flying one
 * takes none. */
void move_apply_knockback(void) {
    u8* flags;
    unit_stats_t* unit;
    action_data_t* action;
    s32 amount;
    s32 diff;

    action_clear_data();
    flags = &g_action_current_ability.knockback_flags;
    *flags &= BATTLE_KNOCKBACK_KIND_MASK;
    g_action_target_data->attack_type = ACTION_TYPE_PSEUDO_STATUS;
    unit = g_action_target;
    amount = 0;
    if (!(unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_CRYSTAL)] & STATUS_BYTE_MASK(STATUS_ID_CRYSTAL))
        && !(unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DEAD)] & STATUS_BYTE_MASK(STATUS_ID_DEAD))
        && !(unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_PETRIFY)] & STATUS_BYTE_MASK(STATUS_ID_PETRIFY))) {
        if (*flags == BATTLE_KNOCKBACK_FALL_DAMAGE) {
            diff = (g_action_current_ability.knockback_fall_height >> 1) - unit->jump;
            if (diff > 0) {
                amount = (diff * unit->max_hp) / 10;
                if (amount >= 0x3E8) {
                    amount = 0x3E7;
                }
            }
        }
        action = g_action_target_data;
        action->hp_damage = amount;
        if (amount != 0) {
            action->attack_type |= ACTION_TYPE_HP_DAMAGE;
        }
    }
}
