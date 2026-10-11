#include "fft/world.h"
#include "psx/types.h"

/* Toggle the portrait flip for the matching message. The record index is
 * uninitialized, so the loop tests the same unspecified buffer six times. */
void world_script_toggle_message_portrait_flip(const u8* parameters) {
    s32 index;
    s32 i;
    s16 value;

    value = world_script_load_halfword(parameters);
    for (i = 0; i < 6; i++) {
        if (g_world_menu_window_buffers[index].message_id == value) {
            g_world_menu_window_buffers[index].portrait_flipped++;
            g_world_menu_window_buffers[index].portrait_flipped &= 1;
        }
    }
}
