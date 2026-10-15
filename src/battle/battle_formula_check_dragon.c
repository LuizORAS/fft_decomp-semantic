#include "fft/battle.h"
#include "psx/types.h"

/* Fail the action (forced failure) unless the target is a dragon or a hydra: monster_graphic 0x0f
 * (Dragon, Blue Dragon, Red Dragon, Holy Dragon) or 0x10 (Hydra, Hyudra, Tiamat). Reis's Dragon
 * abilities check it. */
void battle_formula_check_dragon(void) {
    if ((g_battle_action_target->monster_graphic - 0xF) >= 2U) {
        battle_formula_force_attack_miss();
    }
}
