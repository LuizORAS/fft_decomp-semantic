#include "fft/world.h"

s32 world_menu_update_horizontal_selection_and_mark_change(u16 count, u8 index, s32 input, u8 value) {
    u16 previous;
    s16 current;

    previous = g_world_menu_cursor_positions[index];
    current = world_menu_update_wrapped_horizontal_selection(count, index, input);
    if ((s16)previous != current) {
        g_world_menu_sound_effect_id = value;
    }
    return current;
}
