#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_toggle_message_portrait_flip (FlipMessagePortrait): toggle bit 0 of
 * portrait_flipped of the menu window buffer whose message_id matches the halfword operand.
 *
 * The record index is never initialized in the target: the loop runs six times over whatever slot the
 * uninitialized local names, so the reconstruction reproduces that uninitialized read. */
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
