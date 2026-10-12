#include "fft/battle_formula.h"

/* Formula 0x22: Draw Out status, 100%; katana break roll. */
void formula_draw_out_status(void) {
    formula_calculate_katana_break_chance();
    formula_apply_status_to_action();
}
