#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Start the preview stats window, a second stats panel (world_menu_run_numeric_editor_thread in
 * slot 11 with g_world_preview_stats_thread_params), when `flag` is set, the status banner is not
 * sliding and slot 11 is idle; stop it otherwise. The state goes to g_world_preview_stats_window_active
 * and the style of the stats panel.
 *
 * Built at -O1, like its neighbors world_menu_toggle_stat_preview_panel_thread /
 * world_menu_toggle_ability_panel_thread. */
void world_menu_toggle_preview_stats_window(s32 flag) {
    s32 stored;

    stored = flag;
    if (stored != 0) {
        stored = 1;
        if (!(g_world_status_display_thread_params.flags & (MENU_STATUS_PANEL_SLIDE_DOWN | MENU_STATUS_PANEL_SLIDE_UP))
            && thread_is_running(WORLD_THREAD_SLOT_PREVIEW_STATS) == 0) {
            thread_start(WORLD_THREAD_SLOT_PREVIEW_STATS, world_menu_run_numeric_editor_thread);
            thread_set_parameters(WORLD_THREAD_SLOT_PREVIEW_STATS, (s32)&g_world_preview_stats_thread_params, 0, 0);
        }
    } else {
        world_menu_request_thread_stop(WORLD_THREAD_SLOT_PREVIEW_STATS);
    }
    g_world_numeric_editor_thread_params.style = stored;
    g_world_preview_stats_window_active = stored;
}
