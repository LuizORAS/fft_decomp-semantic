#include "fft/menu.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Thread that runs the unit list overlay (BUNIT.OUT) and returns to the battle;
 * battle_menu_run_bunit_transition_thread starts it in the borrowed fade-out slot
 * once the overlay is loaded and the screen is dark. WORLD compiles this text too.
 *
 * It holds g_battle_menu_transition_state at 4, waits for thread 6 to end, hands
 * the frame driver the overlay's entry point and waits for it to return, frees
 * the high overlay memory, asks threads 2-14 to stop, fades the screen back in
 * over 32 frames and restores the hovered-unit view for the active-turn unit. */
void menu_fade_out_thread(void) {
    s32 i;
    s32 unused[2]; /* keeps the original 0x28 frame */

    g_world_menu_transition_state = 4;
    do {
        thread_yield();
    } while (world_thread_is_running_asm(THREAD_SLOT_MENU_GRANDCHILD) != 0);

    g_world_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_BUNIT;
    g_world_menu_help_open = 0;
    do {
        thread_wait_frames(1);
    } while (g_world_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);

    menu_free_high_overlay();
    for (i = 2; i < 15; i++) {
        thread_set_parameters(i, 0, 0, 1);
    }
    for (i = 0; i < 0x100; i += 8) {
        g_world_screen_fade = 0xFF - i;
        thread_yield();
    }
    world_menu_unblock_help();
    g_menu_overlay_state = 0;
    g_world_screen_fade = 0;
    g_world_menu_transition_state = 0;
    world_unit_start_view_thread(UNIT_VIEW_MODE_HOVERED_UNIT, g_menu_selected_unit.battle_id, 0xFF);
    thread_exit_current();
}
