#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x45, Climhazzard: HP damage equal to the target's max HP - current HP; no evade or hit
 * roll. */
void battle_formula_damage_target_missing_hp(void) {
    battle_stats_t* unit = g_battle_action_target;
    battle_action_data_t* action = g_battle_action_target_data;

    action->hp_damage = unit->max_hp - unit->hp;
    action->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE;
}
