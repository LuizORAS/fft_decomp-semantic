#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x32: damage ((PA + (PA + Y) / 2) * rnd(1..X)); physical evade. */
void formula_damage_random_x_times_pa_plus_y_half_times_pa(void) {
    action_data_t* action;
    u8* mult;
    s32 rnd;

    if (formula_calculate_physical_evade() != 0)
        return;
    formula_store_pa_and_pa_plus_y_divided_by_two();
    formula_apply_attack_up_and_martial_arts();
    formula_apply_physical_status_support_compatibility();
    formula_calculate_critical_hit();
    formula_store_xa_plus_ya_status_damage();
    rnd = formula_get_random_0_7fff();
    mult = &g_action_current_ability.random_damage_factor;
    *mult = (rnd * g_action_current_ability.range_data.x) / 32768 + 1;
    action = g_action_target_data;
    /* The target loads hp_damage signed (lh) here; keep the cast and the
       multiplier deref separated so `* *` never reads as a comment. */
    action->hp_damage = action->hp_damage * (*mult);
    action->attack_type = ACTION_TYPE_HP_DAMAGE;
}
