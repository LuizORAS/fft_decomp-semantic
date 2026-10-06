#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x5C, Dragon Power Up: only on a dragon or a hydra (battle_formula_check_dragon); Brave
 * raised by X and PA, MA and Speed by Y (battle_formula_add_brave_x_stats_y). */
void battle_formula_dragon_brave_x_stats_y(void) {
    battle_formula_check_dragon();
    if (g_battle_action_target_data->hit != 0) {
        battle_formula_add_brave_x_stats_y();
    }
}
