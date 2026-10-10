#include "fft/world.h"

/* Update a wrapping horizontal menu cursor, request sound `value` when it
 * moves and return the new position. */
s32 world_menu_update_horizontal_selection_and_mark_change(u16 count, u8 index, s32 input, u8 value) {
    s16 previous;
    s16 current;

    previous = g_world_menu_cursor_positions[index];
    current = world_menu_update_wrapped_horizontal_selection(count, index, input);
    if (previous != current) {
        g_world_menu_sound_effect_id = value;
    }
    return current;
}
