#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x34: heal (PA*Y) HP and (PA*Y/2) MP. */
void formula_heal_pa_times_y_and_mp(void) {
    action_data_t* action;
    u16 amount;

    formula_store_pa_and_y();
    formula_apply_attack_up_and_martial_arts();
    formula_apply_zodiac_compatibility();
    formula_store_xa_times_ya_damage();
    action = g_action_target_data;
    amount = action->hp_damage;
    action->hp_damage = 0;
    action->hp_healing = amount;
    action->attack_type = ACTION_TYPE_HP_HEALING | ACTION_TYPE_MP_HEALING;
    /* hp_healing is re-read signed (lh) for the halving. */
    action->mp_healing = action->hp_healing / 2;
}
