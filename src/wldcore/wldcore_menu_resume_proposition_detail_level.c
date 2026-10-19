#include "fft/wldcore.h"

/* Resume handler of a proposition's detail: its text render record (the level's second 32-bit value) goes
 * back to CLUT 0. */
void wldcore_menu_resume_proposition_detail_level(wldcore_menu_stack_record_t* level) {
    g_wldcore_window_render_records[level->window_render.render_index].palette = 0;
}
