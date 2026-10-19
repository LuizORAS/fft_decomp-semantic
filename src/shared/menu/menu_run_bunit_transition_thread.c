#include "fft/menu.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Thread that opens the unit list (BUNIT.OUT): fade the screen out over 16
 * frames, suspend the menus, ask the menu and the four unit panel threads to
 * stop, load the overlay's first half, reserve 0x20000 bytes for the high overlay
 * and load the second half there, then start the return thread (menu_fade_out_thread) in the
 * fade-out slot and exit. WORLD compiles this text too. */
void menu_run_bunit_transition_thread(void) {
    s32 level;

    for (level = 0; level < 0x100; level += 0x10) {
        g_world_screen_fade = level;
        thread_yield();
    }
    g_world_screen_fade = 0xFF;
    g_menu_overlay_state = 1;
    thread_set_parameters(THREAD_SLOT_MENU, 0, 0, 1);
    thread_set_parameters(THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
    thread_set_parameters(THREAD_SLOT_UNIT_SUMMARY_PANEL, 0, 0, 1);
    thread_set_parameters(THREAD_SLOT_RIGHT_UNIT_STATS, 0, 0, 1);
    thread_set_parameters(THREAD_SLOT_HOVERED_UNIT_STATS, 0, 0, 1);
    thread_wait_for_unit_view_panels();
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_BUNIT);
    menu_reserve_high_overlay(0x20000);
    menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_BUNIT_SECOND_HALF);
    thread_wait_frames(2);
    g_world_menu_help_open = 1;
    thread_start(THREAD_BORROWED_SLOT_FADE_OUT, world_menu_fade_out_thread);
    thread_exit_current();
}
