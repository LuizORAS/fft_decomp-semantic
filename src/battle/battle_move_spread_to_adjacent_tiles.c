#include "fft/battle.h"

/* Spread the range from the current tile through battle_move_propagate_destination: sources 0-3
 * leave its four sides from the ground, 4-7 from on top of a unit standing there, and 8 and up
 * (outer_count 0x24, when the mover starts on a mount) from that unit's standing height + source
 * / 4 - 2. Each source whose neighbour is on the map tries inner_count destinations. */
void battle_move_spread_to_adjacent_tiles(void) {
    battle_move_spread_state_t* state;
    s32 i;
    s32 j;

    state = g_battle_move_scratch_pad_ptr;
    battle_move_transfer_tiles_height_halves_and_slope_to_scratch_pad();
    for (i = 0; i < state->outer_count; i++) {
        state->source_unit_record_flag = i >= 4;
        g_battle_move_spread_preset_table[i % 4]();
        battle_move_set_spreading_tile_x_and_y_coordinates();
        if ((battle_move_check_spreading_tile_coordinates() == 0) && (battle_move_init_source_panel(i) == 0)) {
            for (j = 0; j < state->inner_count; j++) {
                battle_move_propagate_destination(j, 0);
            }
        }
    }
}
