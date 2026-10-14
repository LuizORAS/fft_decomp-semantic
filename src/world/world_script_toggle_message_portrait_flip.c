#include "fft/menu.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Toggle the portrait flip for the matching message. The record index is
 * uninitialized, so the loop tests the same unspecified buffer six times. */
void world_script_toggle_message_portrait_flip(const u8* parameters) {
    s32 index;
    s32 i;
    s16 value;

    value = script_read_s16(parameters);
    for (i = 0; i < 6; i++) {
        if (g_menu_window_buffers[index].message_id == value) {
            g_menu_window_buffers[index].portrait_flipped++;
            g_menu_window_buffers[index].portrait_flipped &= 1;
        }
    }
}
