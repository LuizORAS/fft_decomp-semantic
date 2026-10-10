#include "fft/battle.h"
#include "psx/types.h"

/* Step -x: the slope shifts select the source's exit side and the
 * destination's entry side. */
void battle_move_spread_to_west_preset(void) {
    battle_move_spread_state_t* spread = g_battle_move_scratch_pad_ptr;
    spread->x_step = -1;
    spread->y_step = 0;
    spread->source_side_shift = 2;
    spread->destination_side_shift = 0;
}
