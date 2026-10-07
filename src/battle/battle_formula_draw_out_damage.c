#include "fft/battle.h"

/* Formula 0x20, Draw Out's damaging katanas (Asura, Koutetsu, Heaven's Cloud, Muramasa,
 * Kikuichimoji, Chirijiraden): the katana break roll (battle_formula_roll_katana_break), then XA = MA
 * and YA = Y as magical damage without Faith (battle_formula_calculate_magical_damage_without_faith).
 * No evade or hit roll. */
void battle_formula_draw_out_damage(void) {
    battle_formula_roll_katana_break();
    battle_formula_store_ma_and_y();
    battle_formula_calculate_magical_damage_without_faith();
}
