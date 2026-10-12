#include "fft/battle_move.h"

/* Step -y: the slope shifts select the source's exit side and the
 * destination's entry side. */
void battle_move_spread_to_south_preset(void) {
    battle_move_spread_state_t* spread = g_battle_move_scratch_pad_ptr;
    spread->y_step = -1;
    spread->source_side_shift = 4;
    spread->x_step = 0;
    spread->destination_side_shift = 6;
}
