#include "fft/battle.h"

/* Formula 0x22, Kiyomori and Masamune: the katana break roll (battle_formula_roll_katana_break),
 * then the katana's status (battle_formula_apply_status_to_action). No evade or hit roll. */
void battle_formula_draw_out_status(void) {
    battle_formula_roll_katana_break();
    battle_formula_apply_status_to_action();
}
