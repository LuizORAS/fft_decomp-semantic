#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Cancel the action on this target: no hit, damage, accuracy or special
 * effect, and a pending weapon proc and knockback are dropped. */
void formula_nullify_action(void) {
    action_data_t* target_data;
    action_data_t* target_data_reload;

    g_action_target_data->hit = 0;
    target_data = g_action_target_data;
    target_data->miss_type = ACTION_MISS_TYPE_NULLIFIED;
    target_data_reload = g_action_target_data;
    target_data->hp_damage = 0;
    /* 16-bit stores clear the byte pairs at 0x2a and 0x10. */
    target_data->attack_accuracy = 0;
    target_data->special_effect = 0;
    target_data_reload->attack_type = 0;
    formula_clear_nullify_flags();
    action_clear_knockback_flag();
}
