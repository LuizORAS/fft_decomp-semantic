#include "fft/menu.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Thread body that closes the unit list (BUNIT) opened from a WORLD menu.
 *
 * Waits for the menu's grandchild thread to end, hands the frame loop the
 * BUNIT overlay state and waits until it is idle again, frees the overlay,
 * asks threads 2..14 to stop, fades the screen in over 32 frames and restarts
 * the hovered-unit view.
 */
void world_menu_fade_out_thread(void) {
    s32 i;
    s32 unused[2]; /* keeps the original 0x28 frame */

    g_world_menu_transition_timeout = 4;
    do {
        thread_yield();
    } while (world_thread_is_running_asm(THREAD_SLOT_MENU_GRANDCHILD) != 0);

    g_world_companion_overlay_state = COMPANION_OVERLAY_STATE_BUNIT;
    g_world_text_overlay_active = 0;
    do {
        thread_wait_frames(1);
    } while (g_world_companion_overlay_state != COMPANION_OVERLAY_STATE_IDLE);

    menu_free_high_overlay();
    for (i = 2; i < 15; i++) {
        thread_set_parameters(i, 0, 0, 1);
    }
    for (i = 0; i < 0x100; i += 8) {
        g_world_screen_fade = 0xFF - i;
        thread_yield();
    }
    world_menu_clear_transition_active_flag();
    g_menu_overlay_state = 0;
    g_world_screen_fade = 0;
    g_world_menu_transition_timeout = 0;
    world_unit_start_view_thread(UNIT_VIEW_MODE_HOVERED_UNIT, g_menu_selected_unit.battle_id, 0xFF);
    thread_exit_current();
}
