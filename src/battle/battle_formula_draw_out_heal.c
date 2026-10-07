#include "fft/battle.h"

/* Formula 0x23, Murasame: the katana break roll (battle_formula_roll_katana_break), then MA * Y
 * restored as HP, or taken as damage by an undead target (battle_formula_apply_undead_reversal). No
 * Faith, evade or hit roll. */
void battle_formula_draw_out_heal(void) {
    battle_formula_roll_katana_break();
    battle_formula_store_ma_and_y();
    battle_formula_store_xa_times_ya_damage();
    battle_formula_apply_undead_reversal();
}
