#include "fft/battle_action.h"
#include "psx/types.h"

/* formula_calculate_physical_evade without the bow weather penalty. */
s32 formula_calculate_physical_evade_charge(void) {
    formula_store_physical_evade_values();
    formula_calculate_concentrate();
    formula_calculate_dark_confuse();
    formula_calculate_abandon();
    formula_apply_evasion_changes_due_to_statuses();
    formula_calculate_facing_evade();
    return formula_calculate_hit();
}
