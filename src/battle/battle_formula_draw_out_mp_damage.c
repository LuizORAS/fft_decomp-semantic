#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x21, Bizen Boat: the katana break roll (battle_formula_roll_katana_break), then MA * Y as
 * MP damage after the magical XA modifiers. No evade or hit roll, element or Faith. */
void battle_formula_draw_out_mp_damage(void) {
    u32 damage;
    battle_action_data_t* action;

    battle_formula_roll_katana_break();
    battle_formula_store_ma_and_y();
    battle_formula_apply_magical_xa_modifiers();
    damage = g_current_ability.xa * g_current_ability.ya;
    action = g_battle_action_target_data;
    action->attack_type = BATTLE_ACTION_TYPE_MP_DAMAGE;
    action->mp_damage = damage;
}
