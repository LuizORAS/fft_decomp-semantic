#include "fft/battle.h"
#include "psx/types.h"

/* Critical hit step: a 4 in 100 roll, never outside execution. XA gains
 * rnd(0..XA-1) and the target may be knocked back. */
void battle_formula_calculate_critical_hit(void) {
    s32 rnd;
    s16 base_xa;
    if (battle_formula_roll_random_at_least(0x64, 4) == 0) {
        g_battle_action_target_data->critical = 1;
        rnd = battle_formula_get_random_0_7fff();
        base_xa = g_battle_action_current_ability.xa;
        g_battle_action_current_ability.xa = base_xa + rnd * base_xa / 32768;
        battle_formula_calculate_knockback();
    }
}
