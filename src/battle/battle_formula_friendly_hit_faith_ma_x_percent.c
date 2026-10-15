#include "fft/battle.h"

/* Formula 0x0B, supporting status spells (Regen, Protect, Shell, Wall, Esuna, Haste, Float,
 * Reflect, Reraise, Carbunkle, Magic Barrier and others): no evade check; hit chance MA + X with the
 * element's Strengthen and both Faiths (battle_formula_calculate_friendly_magic_accuracy), then the
 * spell's status (battle_formula_apply_status_to_action). */
void battle_formula_friendly_hit_faith_ma_x_percent(void) {
    battle_formula_store_ma_and_x();
    battle_formula_apply_ability_element_strengthen();
    if (battle_formula_calculate_friendly_magic_accuracy() == 0) {
        battle_formula_apply_status_to_action();
    }
}
