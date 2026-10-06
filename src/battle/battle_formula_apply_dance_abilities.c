#include "fft/battle.h"
#include "psx/types.h"

/* The Dance effects on a target the dance hit: Witch Hunt takes XA + YA MP and Wiznaibus XA + YA HP,
 * from the dancer's weapon (battle_formula_calculate_base_xa); Slow Dance, Polka Polka and
 * Disillusion lower Speed, PA and MA by 1; Last Dance sets CT to 0; Nameless Dance adds its status
 * (battle_formula_apply_status_to_action). */
void battle_formula_apply_dance_abilities(void) {
    switch (g_current_ability.ability_id) {
    case ABILITY_ID_DANCE_WITCH_HUNT:
        g_battle_action_target_data->mp_damage = g_current_ability.xa + g_current_ability.ya;
        g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_MP_DAMAGE;
        return;
    case ABILITY_ID_DANCE_WIZNAIBUS:
        g_battle_action_target_data->hp_damage = g_current_ability.xa + g_current_ability.ya;
        g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
        return;
    case ABILITY_ID_DANCE_SLOW_DANCE:
        g_battle_action_target_data->sp_change = 1;
        break;
    case ABILITY_ID_DANCE_POLKA_POLKA:
        g_battle_action_target_data->pa_change = 1;
        break;
    case ABILITY_ID_DANCE_DISILLUSION:
        g_battle_action_target_data->ma_change = 1;
        break;
    case ABILITY_ID_DANCE_NAMELESS_DANCE:
        battle_formula_apply_status_to_action();
        return;
    case ABILITY_ID_DANCE_LAST_DANCE:
        g_battle_action_target_data->ct_change = 0x7F;
        break;
    }
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
