#include "fft/world.h"
#include "psx/types.h"

/* Save menu cursor position `index` and return it. */
s16 world_menu_set_cursor_position(u8 index, s16 value) {
    g_world_menu_cursor_positions[index] = value;
    return value;
}
