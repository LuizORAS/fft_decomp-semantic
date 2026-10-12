#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Hit chance of the accuracy formulas: XA + YA, left in hp_damage for
 * formula_use_hp_damage_as_action_hit_percent. YA gets zodiac
 * compatibility here (the callers apply it to XA), except for Golem. */
void formula_store_xa_plus_ya_status_damage(void) {
    u16 original_xa;

    if (g_action_current_ability.ability_id != ABILITY_ID_SUMMON_MAGIC_GOLEM) {
        original_xa = g_action_current_ability.xa;
        g_action_current_ability.xa = g_action_current_ability.ya;
        formula_apply_zodiac_compatibility();
        g_action_current_ability.ya = g_action_current_ability.xa;
        g_action_current_ability.xa = original_xa;
    }
    g_action_target_data->hp_damage = g_action_current_ability.xa + g_action_current_ability.ya;
}
