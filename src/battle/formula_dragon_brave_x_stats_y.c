#include "fft/battle_action.h"

/* Formula 0x5C: +Brave (X), +PA/MA/Speed (Y); dragons only. */
void formula_dragon_brave_x_stats_y(void) {
    formula_check_dragon();
    if (g_action_target_data->hit != 0) {
        formula_add_brave_x_stats_y();
    }
}
