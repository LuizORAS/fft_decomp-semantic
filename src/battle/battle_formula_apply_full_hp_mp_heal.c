#include "fft/battle.h"
#include "psx/types.h"

/* Stage a full restore: the target's max HP as the amount, stored as HP damage until
 * battle_formula_apply_undead_reversal turns it into healing (or keeps it as damage on an undead
 * target), and its max MP as MP healing. */
void battle_formula_apply_full_hp_mp_heal(void) {
    battle_stats_t* unit = g_battle_action_target;
    battle_action_data_t* action = g_battle_action_target_data;

    action->hp_damage = unit->max_hp;
    action->mp_healing = unit->max_mp;
    action->attack_type = BATTLE_ACTION_TYPE_HP_DAMAGE | BATTLE_ACTION_TYPE_MP_HEALING;
}
