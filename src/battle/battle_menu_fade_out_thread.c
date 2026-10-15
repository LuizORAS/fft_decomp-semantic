#include "fft/battle.h"
#include "psx/types.h"

/* Thread that runs the unit list overlay (BUNIT.OUT) and returns to the battle;
 * battle_menu_run_bunit_transition_thread starts it in the borrowed fade-out slot
 * once the overlay is loaded and the screen is dark.
 *
 * It holds g_battle_menu_transition_state at 4, waits for thread 6 to end, hands
 * the frame driver the overlay's entry point and waits for it to return, frees
 * the high overlay memory, asks threads 2-14 to stop, fades the screen back in
 * over 32 frames and restores the hovered-unit view for the active-turn unit. */
void battle_menu_fade_out_thread(void) {
    s32 thread_id;
    s32 fade;
    s32 frame_padding[2]; /* Unused; reproduces the target's 8 extra frame bytes. */

    g_battle_menu_transition_state = 4;
    do {
        battle_thread_yield();
    } while (battle_thread_is_running_asm(NATIVE_THREAD_SLOT_MENU_GRANDCHILD));

    g_battle_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_BUNIT;
    g_battle_menu_help_open = 0;
    do {
        battle_thread_wait_frames(1);
    } while (g_battle_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);

    battle_menu_free_high_overlay();
    for (thread_id = 2; thread_id < 15; thread_id++) {
        battle_thread_set_parameters(thread_id, 0, 0, 1);
    }

    for (fade = 0; fade < 256; fade += 8) {
        g_battle_screen_fade = 0xff - fade;
        battle_thread_yield();
    }

    battle_menu_unblock_help();
    g_battle_menu_suspend_state = 0;
    g_battle_screen_fade = 0;
    g_battle_menu_transition_state = 0;
    battle_unit_start_view_thread(UNIT_VIEW_MODE_HOVERED_UNIT, g_battle_active_turn_unit.battle_id, 0xff);
    battle_thread_exit_current();
}
