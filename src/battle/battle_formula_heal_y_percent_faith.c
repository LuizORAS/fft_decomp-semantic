#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x0D, Raise and Raise 2: no evade check; hit chance MA + X with both Faiths; the spell's
 * status must change something unless the target is undead
 * (battle_formula_apply_status_and_check_undead); then Y% of max HP restored, or taken as damage by
 * an undead target. */
void battle_formula_heal_y_percent_faith(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_ability_element_strengthen();
    if (battle_formula_calculate_friendly_magic_accuracy() != 0) {
        return;
    }
    if (battle_formula_apply_status_and_check_undead() == 0) {
        return;
    }
    battle_formula_calculate_hp_percent_damage();
    battle_formula_apply_undead_reversal();
}
