#include "fft/world.h"

/* Bring the status banner back after an egg: an egg hides it, and a unit staged while it is hidden
 * sets g_world_formation_unit_slide_pending (world_formation_stage_selected_unit). Count nine frames
 * with no redraw, then show the banner (flags 0) and ask it to redraw (redraw_request 1).
 *
 * Compiled at -O1 like its neighbours: the final arm's two field stores share
 * one `la` of redraw_request, with flags at -4 from it. At -O2, cse2 folds that
 * -4 store back into an absolute address and the scheduler moves the first
 * arm's store into the frame load's delay slot. */
void world_formation_step_unit_selection_slide(void) {
    if (g_world_formation_unit_slide_frame < 9) {
        g_world_formation_unit_slide_frame = g_world_formation_unit_slide_frame + 1;
        g_world_status_display_thread_params.redraw_request = 0;
    } else {
        g_world_formation_unit_slide_frame = 0;
        g_world_formation_unit_slide_pending = 0;
        g_world_status_display_thread_params.redraw_request = 1;
        g_world_status_display_thread_params.flags = 0;
    }
}
