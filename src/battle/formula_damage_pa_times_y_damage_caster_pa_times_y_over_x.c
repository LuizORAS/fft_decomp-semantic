#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "psx/types.h"

/* Formula 0x42: damage (PA * Y); the caster takes (PA * Y / X). No evade. */
void formula_damage_pa_times_y_damage_caster_pa_times_y_over_x(void) {
    s16 damage;
    action_data_t* action;

    formula_store_pa_and_y();
    formula_apply_zodiac_compatibility();
    damage = g_action_current_ability.xa * g_action_current_ability.ya;
    /* Cast-pointer stores: as struct members GCC sinks them past the
     * following scalar global loads; the target keeps source order. */
    g_action_target_data->hp_damage = damage;
    damage = damage / g_action_current_ability.range_data.x;
    action = g_action_attacker_data;
    action->hit = 1;
    action->hp_damage = damage;
    g_action_target_data->attack_type = ACTION_TYPE_HP_DAMAGE;
    g_action_attacker_data->attack_type = ACTION_TYPE_HP_DAMAGE;
}
