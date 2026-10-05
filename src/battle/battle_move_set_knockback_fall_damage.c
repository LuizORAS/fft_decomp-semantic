#include "fft/battle.h"
#include "psx/types.h"

/* Resolve a pending knockback (knockback_flags bit 0x80, cleared here) as the target's action
 * result: no damage, unless a ground knockback (kind 1; kind 2 is a flier) of a target that is not
 * crystallized, dead or petrified falls more levels than its Jump: (levels - Jump) * Max HP / 10, at
 * most 999. battle_move_check_knockback_destination records the fall in half levels, and
 * battle_action_run_pre_formula_setup calls this while bit 0x80 is set. */
void battle_move_set_knockback_fall_damage(void) {
    u8* flags;
    battle_stats_t* unit;
    battle_action_data_t* action;
    s32 amount;
    s32 diff;

    battle_action_clear_data();
    flags = &g_current_ability.knockback_flags;
    *flags &= 0x7F;
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
    unit = g_battle_action_target;
    amount = 0;
    if ((*(u16*)&unit->status_sets.current[0]
            & (BATTLE_STATUS_PACKED_MASK(BATTLE_STATUS_ID_CRYSTAL) | BATTLE_STATUS_PACKED_MASK(BATTLE_STATUS_ID_DEAD)
                | BATTLE_STATUS_PACKED_MASK(BATTLE_STATUS_ID_PETRIFY)))
        == 0) {
        if (*flags == 1) {
            diff = (g_current_ability.knockback_fall_height >> 1) - unit->jump;
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
