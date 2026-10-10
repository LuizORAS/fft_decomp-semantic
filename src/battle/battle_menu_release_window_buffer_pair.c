#include "fft/battle.h"
#include "psx/types.h"

/* Release the menu window buffer pair whose first buffer is `buffer`: both slot
 * pointers become -1 and the active and thread_id words of both buffers are
 * cleared. -1 is ignored; a buffer that is not the first of one of the three
 * pairs stops the current thread.
 *
 * BATTLE twin of world_menu_release_window_buffer_pair. */
void battle_menu_release_window_buffer_pair(s32 buffer) {
    s32 slot;
    s32 empty;

    if (buffer == -1) {
        return;
    }

    slot = 0;
    empty = -1;
    do {
        if ((s32)g_battle_menu_buffer_slots[slot] == buffer) {
            g_battle_menu_buffer_slots[slot] = (void*)empty;
            g_battle_menu_buffer_slots[slot + 1] = (void*)empty;
            g_battle_menu_window_buffers[slot].active = 0;
            g_battle_menu_window_buffers[slot + 1].active = 0;
            g_battle_menu_window_buffers[slot].thread_id = 0;
            g_battle_menu_window_buffers[slot + 1].thread_id = 0;
            return;
        }
        slot += 2;
    } while (slot < 6);

    battle_thread_exit_current();
}
