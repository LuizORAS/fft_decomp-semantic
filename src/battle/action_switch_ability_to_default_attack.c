#include "fft/battle_action.h"
#include "psx/types.h"

/* Re-run the pre-formula setup against the current target as a plain weapon
 * Attack, then restore the attacker's skillset and ability and the strike
 * counter. It ends with the current ability's weapon_proc_pending and reaction_id
 * cleared. */
void action_switch_ability_to_default_attack(void) {
    unit_stats_t* unit;
    unit_stats_t* u2;
    unit_stats_t* u3;
    u8 saved_strike_counter;
    u8 saved_skillset;
    u16 saved_ability;

    unit = g_action_attacker;
    saved_strike_counter = g_action_current_ability.strike_counter;
    saved_skillset = unit->action_command.skillset_id;
    unit->action_command.skillset_id = SKILLSET_ID_ATTACK;
    u2 = g_action_attacker;
    saved_ability = unit->action_command.ability_id;
    u2->action_command.ability_id = 0;
    action_run_pre_formula_setup(&u2->action_command, g_action_current_ability.target_id);
    u3 = g_action_attacker;
    g_action_current_ability.strike_counter = saved_strike_counter;
    u3->action_command.ability_id = saved_ability;
    u3->action_command.skillset_id = saved_skillset;
    g_action_current_ability.weapon_proc_pending = 0;
    g_action_current_ability.reaction_id = 0;
}
