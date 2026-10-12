#include "fft/battle_formula.h"

/* Formula 0x54: heal (MA * Y) MP. */
void formula_heal_mp_ma_times_y(void) {
    formula_store_ma_and_y();
    formula_calculate_magical_xa_times_ya();
    formula_convert_hp_damage_to_mp_recovery();
}
