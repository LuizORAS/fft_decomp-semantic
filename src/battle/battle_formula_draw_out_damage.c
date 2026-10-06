#include "fft/battle.h"

void battle_formula_draw_out_damage(void) {
    battle_formula_roll_katana_break();
    battle_formula_store_ma_and_y();
    battle_formula_calculate_magical_damage_without_faith();
}
