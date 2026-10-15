#include "fft/battle.h"

/* Formula 0x54, Magic Spirit: no evade or hit roll; MA * Y with Magic Attack Up and the zodiac,
 * without Faith, restored as MP (battle_formula_convert_hp_damage_to_mp_recovery). */
void battle_formula_heal_mp_ma_times_y(void) {
    battle_formula_store_ma_and_y();
    battle_formula_calculate_magical_xa_times_ya();
    battle_formula_convert_hp_damage_to_mp_recovery();
}
