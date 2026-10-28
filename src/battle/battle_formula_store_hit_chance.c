#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Store the hit chance XA + YA in the target's hp_damage, where the accuracy steps keep it until
 * battle_formula_roll_hit_chance rolls it. YA gets the zodiac compatibility too,
 * except for the Golem summon. */
void battle_formula_store_hit_chance(void) {
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
