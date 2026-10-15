#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x4A, Elixir: restore all HP and MP (battle_formula_apply_full_hp_mp_heal); an undead
 * target takes the HP part as damage. */
void battle_formula_heal_full_hp_mp(void) {
    battle_formula_apply_full_hp_mp_heal();
    battle_formula_apply_undead_reversal();
}
