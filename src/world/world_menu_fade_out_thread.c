#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_fade_out_thread. */
void world_menu_fade_out_thread(void) {
    s32 i;
    s32 unused[2]; /* keeps the original 0x28 frame */

    g_world_menu_transition_state = 4;
    do {
        world_thread_yield();
    } while (world_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_GRANDCHILD) != 0);

    g_world_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_BUNIT;
    g_world_menu_help_open = 0;
    do {
        world_thread_wait_frames(1);
    } while (g_world_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);

    world_menu_free_high_overlay();
    for (i = 2; i < 15; i++) {
        world_thread_set_parameters(i, 0, 0, 1);
    }
    for (i = 0; i < 0x100; i += 8) {
        g_world_screen_fade = 0xFF - i;
        world_thread_yield();
    }
    world_menu_unblock_help();
    g_world_menu_suspend_state = 0;
    g_world_screen_fade = 0;
    g_world_menu_transition_state = 0;
    world_unit_start_view_thread(UNIT_VIEW_MODE_HOVERED_UNIT, g_world_active_turn_unit.battle_id, 0xFF);
    world_thread_exit_current();
}
