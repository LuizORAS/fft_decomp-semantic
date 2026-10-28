#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Attack Up (XA * 4 / 3) and, bare-handed, Martial Arts (XA * 3 / 2) for the physical abilities
 * that use PA; weapon strikes take battle_formula_apply_physical_attack_supports instead. */
void battle_formula_apply_attack_up_and_martial_arts(void) {
    if (g_battle_action_attacker->ability_flags.support[1] & BATTLE_SUPPORT_SET_2_ATTACK_UP) {
        g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 4 / 3;
    }

    {
        u8 weapon_id = g_battle_action_current_ability.weapon_id;
        battle_stats_t* attacker = g_battle_action_attacker;
        u8 weapon_type = g_main_item_primary_data[weapon_id].type;
        u8 has_martial_arts = attacker->ability_flags.support[2] & BATTLE_SUPPORT_SET_3_MARTIAL_ARTS;

        if (has_martial_arts && weapon_type == ITEM_TYPE_UNARMED) {
            g_battle_action_current_ability.xa = g_battle_action_current_ability.xa * 3 / 2;
        }
    }
}
