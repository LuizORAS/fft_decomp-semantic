#include "fft/battle.h"
#include "psx/types.h"

/* Re-run the pre-formula setup against the current target as a plain weapon
 * Attack, then restore the attacker's skillset and ability and the strike
 * counter. It ends with the current ability's weapon_proc_pending and reaction_id
 * cleared. */
void battle_action_switch_ability_to_default_attack(void) {
    battle_stats_t* unit;
    battle_stats_t* u2;
    battle_stats_t* u3;
    u8* pc3;
    u8 saved_c3;
    u8 saved_skillset;
    u16 saved_ability;

    unit = g_battle_action_attacker;
    pc3 = &g_battle_action_current_ability.strike_counter;
    saved_c3 = *pc3;
    saved_skillset = unit->action_command.skillset_id;
    unit->action_command.skillset_id = SKILLSET_ID_ATTACK;
    u2 = g_battle_action_attacker;
    saved_ability = unit->action_command.ability_id;
    u2->action_command.ability_id = 0;
    battle_action_run_pre_formula_setup((u8*)&u2->action_command, g_battle_action_current_ability.target_id);
    u3 = g_battle_action_attacker;
    *pc3 = saved_c3;
    u3->action_command.ability_id = saved_ability;
    u3->action_command.skillset_id = saved_skillset;
    g_battle_action_current_ability.weapon_proc_pending = 0;
    g_battle_action_current_ability.reaction_id = 0;
}
