#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x0F, Spell Absorb and Aspel: the magical evade check and hit chance (MA + X with both
 * Faiths, no Strengthen), then Y% of the target's max MP drained to the attacker
 * (battle_formula_apply_mp_absorption). */
void battle_formula_absorb_mp_y_percent(void) {
    if (battle_formula_calculate_magical_evade() != 0) {
        return;
    }
    if (battle_formula_calculate_magic_accuracy_without_strengthen() != 0) {
        return;
    }
    battle_formula_calculate_mp_percent_damage();
    g_battle_action_target_data->hp_damage = g_battle_action_target_data->mp_damage;
    battle_formula_apply_mp_absorption();
}
