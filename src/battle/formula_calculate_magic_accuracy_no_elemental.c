#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* formula_calculate_magic_accuracy without the elemental strengthen. */
s32 formula_calculate_magic_accuracy_no_elemental(void) {
    formula_store_ma_and_x();
    formula_apply_magical_support_status_compatibility();
    formula_store_xa_plus_ya_status_damage();
    formula_calculate_faith();
    formula_use_hp_damage_as_action_hit_percent();
    return g_action_target_data->hit == 0;
}
