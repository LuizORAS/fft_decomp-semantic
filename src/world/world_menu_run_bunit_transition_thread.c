#include "fft/world.h"
#include "psx/types.h"

/*
 * Thread body that opens the unit list (BUNIT) from a WORLD menu.
 *
 * Fades the screen out over 16 frames, asks the menu and panel threads to
 * stop and waits for slots 10..13, loads both halves of the overlay around a
 * 0x20000-byte reservation at the high overlay address, and hands over to
 * world_menu_fade_out_thread, which runs the overlay and fades back in.
 */
void world_menu_run_bunit_transition_thread(void) {
    s32 level;

    for (level = 0; level < 0x100; level += 0x10) {
        g_world_screen_fade = level;
        world_thread_yield();
    }
    g_world_screen_fade = 0xFF;
    g_world_menu_suspend_state = 1;
    world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS, 0, 0, 1);
    world_thread_wait_for_unit_view_panels();
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_BUNIT);
    world_menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_BUNIT_SECOND_HALF);
    world_thread_wait_frames(2);
    g_world_menu_help_open = 1;
    world_thread_start(NATIVE_BORROWED_THREAD_SLOT_FADE_OUT, world_menu_fade_out_thread);
    world_thread_exit_current();
}
