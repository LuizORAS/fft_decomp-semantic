#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x43: damage equal to the caster's missing HP (MaxHP - CurHP). */
void formula_43_damage_caster_missing_hp(void) {
    battle_stats_t* unit = g_action_attacker;
    action_data_t* action = g_action_target_data;
    u16 max_hp = unit->max_hp;
    u16 cur_hp = unit->hp;
    action->attack_type = ACTION_TYPE_HP_DAMAGE;
    action->hp_damage = max_hp - cur_hp;
}
