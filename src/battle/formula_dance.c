#include "fft/battle_action.h"

/* Formula 0x1D: dance, hit X%: (weapon XA + YA) MP or HP damage, -1 Speed, PA
 * or MA, status, or CT to 0 by ability. Never hits a sleeping target. */
void formula_dance(void) {
    formula_force_sleeping_target_miss();
    if (g_action_target_data->hit != 0) {
        if (formula_calculate_dance_song_hit() == 0) {
            formula_calculate_base_xa();
            formula_apply_dance_abilities();
        }
    }
}
