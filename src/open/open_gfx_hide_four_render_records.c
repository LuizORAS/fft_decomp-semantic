#include "fft/open.h"

/* Hide the four title menu entries. */
void open_gfx_hide_four_render_records(open_title_controller_t* title) {
    g_open_gfx_render_records_36[title->records_36[1]].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_open_gfx_render_records_36[title->records_36[2]].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_open_gfx_render_records_36[title->records_36[3]].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_open_gfx_render_records_36[title->records_36[4]].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
}
