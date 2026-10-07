#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x1C, the Bard's songs: a sleeping target fails (battle_formula_force_sleeping_target_miss);
 * otherwise no evade check, an X% hit (battle_formula_calculate_dance_song_hit), XA = MA and YA = Y,
 * and the song's effect (battle_formula_apply_song_abilities). */
void battle_formula_song(void) {
    battle_formula_force_sleeping_target_miss();
    if (g_battle_action_target_data->hit != 0) {
        if (battle_formula_calculate_dance_song_hit() == 0) {
            battle_formula_store_ma_and_y();
            battle_formula_apply_song_abilities();
        }
    }
}
