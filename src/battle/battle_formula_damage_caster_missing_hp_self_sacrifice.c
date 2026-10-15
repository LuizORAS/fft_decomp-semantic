#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x52, Self Destruct: no evade or hit roll; every target but the caster takes damage equal
 * to the caster's max HP - current HP and the ability's status; the caster, when in the area, takes
 * damage equal to its own current HP. */
void battle_formula_damage_caster_missing_hp_self_sacrifice(void) {
    if (g_battle_action_target != g_battle_action_attacker) {
        battle_stats_t* caster = g_battle_action_attacker;
        battle_action_data_t* action = g_battle_action_target_data;

        action->hp_damage = caster->max_hp - caster->hp;
        action->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
    } else {
        battle_action_data_t* action = g_battle_action_target_data;

        action->hp_damage = g_battle_action_target->hp;
        action->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
    }
    if (g_battle_action_target != g_battle_action_attacker) {
        battle_formula_apply_status();
    }
}
