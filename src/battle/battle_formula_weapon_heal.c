#include "fft/battle.h"

/* Formula 0x07, the Healing Staff: XA and YA by weapon type, the zodiac compatibility, Charge's
 * power and the physical XA modifiers, then XA * YA restored as HP, or taken as damage by an undead
 * target (battle_formula_apply_undead_reversal). It cannot miss and triggers no reactions. */
void battle_formula_weapon_heal(void) {
    battle_formula_calculate_base_xa();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_apply_charge();
    battle_formula_apply_physical_xa_modifiers();
    battle_formula_store_xa_times_ya_damage();
    battle_formula_apply_undead_reversal();
}
