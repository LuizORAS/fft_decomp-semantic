#include "fft/world.h"
#include "psx/types.h"

void world_menu_run_bunit_transition_thread(void) {
    s32 level;

    for (level = 0; level < 0x100; level += 0x10) {
        g_world_screen_fade = level;
        world_thread_yield();
    }
    g_world_screen_fade = 0xFF;
    g_world_menu_overlay_state = 1;
    world_thread_set_parameters(NATIVE_THREAD_SLOT_MENU, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_TILE_INFO_PANEL, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS, 0, 0, 1);
    world_thread_set_parameters(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS, 0, 0, 1);
    world_thread_wait_for_10_to_13();
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_BUNIT);
    world_menu_reserve_high_overlay(0x20000);
    world_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_BUNIT_SECOND_HALF);
    world_thread_wait_frames(2);
    g_world_text_overlay_active = 1;
    world_thread_start(NATIVE_BORROWED_THREAD_SLOT_FADE_OUT, world_menu_fade_out_thread);
    world_thread_exit_current();
}
