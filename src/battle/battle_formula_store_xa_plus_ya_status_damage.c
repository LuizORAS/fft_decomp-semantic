#include "fft/battle.h"
#include "psx/types.h"

/* Hit chance of the accuracy formulas: XA + YA, left in hp_damage for
 * battle_formula_use_hp_damage_as_action_hit_percent. YA gets zodiac
 * compatibility here (the callers apply it to XA), except for Golem. */
void battle_formula_store_xa_plus_ya_status_damage(void) {
    u16 original_xa;

    if (g_battle_action_current_ability.ability_id != ABILITY_ID_SUMMON_MAGIC_GOLEM) {
        original_xa = g_battle_action_current_ability.xa;
        g_battle_action_current_ability.xa = g_battle_action_current_ability.ya;
        battle_formula_apply_zodiac_compatibility();
        g_battle_action_current_ability.ya = g_battle_action_current_ability.xa;
        g_battle_action_current_ability.xa = original_xa;
    }
    g_battle_action_target_data->hp_damage = g_battle_action_current_ability.xa + g_battle_action_current_ability.ya;
}
