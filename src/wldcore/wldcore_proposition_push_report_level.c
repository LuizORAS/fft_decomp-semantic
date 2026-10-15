#include "fft/wldcore.h"

/* Push a returned proposition's report (WLDCORE_MENU_LEVEL_PROPOSITION_REPORT) over the Bar's
 * hidden windows, with the report music; for units that were called back it starts at state 99,
 * which skips the report and goes straight to the result.
 *
 * The subscripted lvalues keep both scalars in-struct for GCC's alias
 * classification, which holds the g_wldcore_proposition_outcome.gate load and the ordering-table store
 * after the menu-record stores as in the target; plain scalars are hoisted. */
void wldcore_proposition_push_report_level(s32 argument) {
    s32 depth;

    depth = g_wldcore_menu_stack_depth;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.first_window].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.second_window].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[g_wldcore_menu_stack_records_next[depth - 1].window_pair_render.render_index].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_menu_stack_records_next[depth].proposition_report.state = -1;
    g_wldcore_menu_stack_records_next[depth].proposition_report.proposition = argument;
    g_wldcore_menu_stack_records_next[depth].proposition_report.phase = 0;
    g_wldcore_menu_stack_records_next[depth].proposition_report.load_phase = WLDCORE_PROPOSITION_REPORT_LOAD_MESSAGES;
    g_wldcore_menu_stack_records_next[depth].proposition_report.render_index = -1;
    (&g_wldcore_menu_ordering_table_offset)[0] = 9;
    if (g_wldcore_proposition_outcome.gate != 0) {
        g_wldcore_menu_stack_records_next[depth].proposition_report.state = 0x63;
    } else {
        wldcore_sound_enqueue_music_start(WLDCORE_AUDIO_MUSIC(2, MAIN_MUSIC_PROPOSITION_REPORT));
    }
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_REPORT;
    g_wldcore_menu_stack_depth++;
}
