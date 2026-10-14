#include "fft/open.h"

/* Show the four 36-byte records whose indices are words 1 to 4 of a controller
 * record. The caller passes the title menu's record, where they are the four menu
 * entries. */
void open_gfx_show_four_render_records(open_title_entry_records_t* group) {
    g_open_gfx_render_records_36[group->indices[0]].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_open_gfx_render_records_36[group->indices[1]].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_open_gfx_render_records_36[group->indices[2]].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_open_gfx_render_records_36[group->indices[3]].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
}
