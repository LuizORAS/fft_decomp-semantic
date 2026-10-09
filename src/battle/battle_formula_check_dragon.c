#include "fft/battle.h"
#include "psx/types.h"

void battle_formula_check_dragon(void) {
    if ((g_battle_action_target->graphic_variant - 0xF) >= 2U) {
        battle_formula_force_attack_miss();
    }
}
