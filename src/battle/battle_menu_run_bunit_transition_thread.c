#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "fft/battle_thread.h"
#include "fft/battle_unit.h"
#include "fft/menu.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Thread that opens the unit list (BUNIT.OUT): fade the screen out over 16
 * frames, suspend the menus, ask the menu and the four unit panel threads to
 * stop, load the overlay's first half, reserve 0x20000 bytes for the high overlay
 * and load the second half there, then start battle_menu_fade_out_thread in the
 * fade-out slot and exit. */
void battle_menu_run_bunit_transition_thread(void) {
    s32 fade;

    for (fade = 0; fade < 256; fade += 16) {
        g_battle_screen_fade = fade;
        thread_yield();
    }
    g_battle_screen_fade = 0xff;
    g_battle_menu_suspend_state = 1;
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL, 0, 0, 1);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS, 0, 0, 1);
    battle_thread_set_parameters(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS, 0, 0, 1);
    battle_thread_wait_for_unit_view_panels();
    battle_menu_request_open_companion_executable(COMPANION_OVERLAY_BUNIT);
    menu_reserve_high_overlay(0x20000);
    battle_menu_request_open_companion_executable(COMPANION_OVERLAY_BUNIT_SECOND_HALF);
    battle_thread_wait_frames(2);
    g_battle_menu_help_open = 1;
    battle_thread_start(NATIVE_BORROWED_THREAD_SLOT_FADE_OUT, battle_menu_fade_out_thread);
    battle_thread_exit_current();
}
