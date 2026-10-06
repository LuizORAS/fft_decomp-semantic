#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x34, Chakra: no evade or hit roll; PA * Y with Attack Up and Martial Arts and the zodiac,
 * restored as HP, and half of it as MP. There is no undead reversal, so an undead target heals too. */
void battle_formula_heal_pa_times_y_and_mp(void) {
    battle_action_data_t* action;
    u16 amount;

    battle_formula_store_pa_and_y();
    battle_formula_apply_attack_up_and_martial_arts();
    battle_formula_apply_zodiac_compatibility();
    battle_formula_store_xa_times_ya_damage();
    action = g_battle_action_target_data;
    amount = action->hp_damage;
    action->hp_damage = 0;
    action->hp_healing = amount;
    action->attack_type = BATTLE_ACTION_TYPE_HP_HEALING | BATTLE_ACTION_TYPE_MP_HEALING;
    action->mp_healing = action->hp_healing / 2;
}
