#include "fft/battle.h"

/* Formula 0x20: Draw Out damage (MA * Y) with element; no evade; 19% status;
 * katana break roll. */
void battle_formula_draw_out_damage(void) {
    battle_formula_calculate_katana_break_chance();
    battle_formula_store_ma_and_y();
    battle_formula_calculate_truth_magical_damage();
}
