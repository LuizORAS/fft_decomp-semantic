#include "fft/battle_action.h"
#include "psx/types.h"

/* Formula 0x21: Draw Out MP damage (MA * Y); katana break roll. */
void battle_formula_draw_out_mp_damage(void) {
    s16 damage;
    action_data_t* action;

    battle_formula_calculate_katana_break_chance();
    battle_formula_store_ma_and_y();
    battle_formula_apply_magical_support_status_compatibility();
    damage = g_action_current_ability.xa * g_action_current_ability.ya;
    action = g_action_target_data;
    action->attack_type = ACTION_TYPE_MP_DAMAGE;
    action->mp_damage = damage;
}
