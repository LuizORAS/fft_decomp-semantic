#include "fft/battle_action.h"
#include "psx/types.h"

void formula_convert_hp_damage_to_mp_recovery(void) {
    action_data_t* action;
    u16 hp_damage;

    action = g_action_target_data;
    hp_damage = action->hp_damage;
    action->hp_damage = 0;
    action->attack_type = ACTION_TYPE_MP_HEALING;
    action->mp_healing = hp_damage;
}
