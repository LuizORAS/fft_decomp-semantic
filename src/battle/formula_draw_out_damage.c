#include "fft/battle_action.h"

/* Formula 0x20: Draw Out damage (MA * Y) with element; no evade; 19% status;
 * katana break roll. */
void formula_draw_out_damage(void) {
    formula_calculate_katana_break_chance();
    formula_store_ma_and_y();
    formula_calculate_truth_magical_damage();
}
