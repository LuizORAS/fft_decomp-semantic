#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x1C: song, hit X%: heal (MA + Y) MP or HP, +1 Speed, PA or MA,
 * status, or Quick by ability. Never hits a sleeping target. */
void formula_song(void) {
    formula_force_sleeping_target_miss();
    if (g_action_target_data->hit != 0) {
        if (formula_calculate_dance_song_hit() == 0) {
            formula_store_ma_and_y();
            formula_apply_song_abilities();
        }
    }
}
