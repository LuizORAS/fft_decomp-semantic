#include "fft/event_require.h"
#include "psx/types.h"

/*
 * Thread body: suspend thread g_require_thread_suspended_id and draw the
 * gradient grid and status group primitives every frame in its place until
 * the third thread parameter is set.
 *
 * The suspended thread's record is copied aside first and copied back before
 * the thread is resumed.
 */
void require_gfx_run_suspended_thread_transition(void) {
    s32 frame = 0;
    u8* saved;

    battle_thread_suspend(g_require_thread_suspended_id);
    saved = battle_menu_alloc_memory(NATIVE_THREAD_STRIDE);
    battle_copy_bytes(saved, &g_battle_threads[g_require_thread_suspended_id], NATIVE_THREAD_STRIDE);
    g_require_panel_status_group_count = 2;
    while (1) {
        s32 buffer = frame & 1;
        require_gfx_build_gradient_grid_primitives(g_require_gfx_poly_gt4_buffers[buffer]);
        require_gfx_build_status_group_primitives(g_require_gfx_poly_ft4_buffers[buffer]);
        battle_thread_yield();
        if (battle_thread_get_current_parameter_3() != 0) {
            break;
        }
        frame++;
    }
    battle_copy_bytes(&g_battle_threads[g_require_thread_suspended_id], saved, NATIVE_THREAD_STRIDE);
    battle_menu_free_memory(saved);
    battle_thread_resume(g_require_thread_suspended_id);
    battle_thread_yield();
    battle_thread_exit_current();
}
