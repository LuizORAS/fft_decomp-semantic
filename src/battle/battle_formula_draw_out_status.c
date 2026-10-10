#include "fft/battle.h"

/* Formula 0x22: Draw Out status, 100%; katana break roll. */
void battle_formula_draw_out_status(void) {
    battle_formula_calculate_katana_break_chance();
    battle_formula_apply_status_to_action();
}
