#include "fft/world.h"

/* Update a wrapping menu cursor and request a sound when it moves. */
s32 world_menu_update_vertical_selection_and_mark_change(u16 count, u8 cursor_id, s32 input, u8 sound) {
    s16 previous;
    s32 current;

    previous = g_world_menu_cursor_positions[cursor_id];
    current = world_menu_update_wrapped_vertical_selection(count, cursor_id, input);
    if (previous != current) {
        g_world_menu_sound_effect_id = sound;
    }
    return current;
}
