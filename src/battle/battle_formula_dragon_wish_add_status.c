#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x5B, Dragon Care: only on a dragon or a hydra (battle_formula_check_dragon); formula
 * 0x3C's exchange (the caster loses a fifth of its max HP and the target restores twice that), then
 * the ability's status (battle_formula_apply_status). */
void battle_formula_dragon_wish_add_status(void) {
    battle_formula_check_dragon();
    if (g_battle_action_target_data->hit != 0) {
        battle_formula_3c_damage_caster_max_hp_one_fifth_heal_target_two_fifths();
        battle_formula_apply_status();
    }
}
