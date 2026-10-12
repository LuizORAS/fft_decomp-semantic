#include "fft/battle_action.h"

/* Miss unless the target's monster graphic is 0x0F or 0x10: the dragon and
 * hydra jobs and the Holy Dragon. */
void formula_check_dragon(void) {
    if ((g_action_target->monster_graphic - 0xF) >= 2U) {
        formula_force_attack_miss();
    }
}
