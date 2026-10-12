#include "fft/battle_move.h"

/* Load the current tile's base height (doubled, in half-height units), half
 * height and slope into the spread state. */
void move_transfer_tiles_height_halves_and_slope_to_scratch_pad(void) {
    move_spread_state_t* state = g_move_scratch_pad_ptr;
    state->source_base_height_times_two = state->current_tile->height << 1;
    state->source_half_height = state->current_tile->depth_half_height.half_height;
    state->source_slope = state->current_tile->slope_type;
}
