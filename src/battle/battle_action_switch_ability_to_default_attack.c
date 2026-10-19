#include "fft/battle_action.h"
#include "psx/types.h"

/* Run the formula setup again as a plain Attack (skillset Attack, ability 0) against the current
 * target, then restore the strike counter, skillset and ability and drop any pending weapon
 * spell and reaction id. The Break formula (battle_formula_break_equipped_hit_pa_wp_y_percent)
 * falls back to it when the target has nothing to break
 * (battle_formula_select_target_equipment). */
void battle_action_switch_ability_to_default_attack(void) {
    battle_stats_t* unit;
    battle_stats_t* u2;
    battle_stats_t* u3;
    u8 saved_strike_counter;
    u8 saved_skillset;
    u16 saved_ability;

    unit = g_battle_action_attacker;
    saved_strike_counter = g_battle_action_current_ability.strike_counter;
    saved_skillset = unit->action_command.skillset_id;
    unit->action_command.skillset_id = SKILLSET_ID_ATTACK;
    u2 = g_battle_action_attacker;
    saved_ability = unit->action_command.ability_id;
    u2->action_command.ability_id = 0;
    battle_action_run_pre_formula_setup(&u2->action_command, g_battle_action_current_ability.target_id);
    u3 = g_battle_action_attacker;
    g_battle_action_current_ability.strike_counter = saved_strike_counter;
    u3->action_command.ability_id = saved_ability;
    u3->action_command.skillset_id = saved_skillset;
    g_battle_action_current_ability.weapon_spell_pending = 0;
    g_battle_action_current_ability.reaction_id = 0;
}
