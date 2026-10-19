#include "fft/wldcore.h"

/* Put the status help cursor window on its field (the field's x + 14 and y + 12) and, when the field's
 * kind changes, switch cursors: kind 0 the idle cursor, any other WLDCORE_ANIM_SEQUENCE_STATUS_HELP_CURSOR.
 *
 * Routing both stores through one base variable (rather than `&g_wldcore_window_record_positions[index]`)
 * is what reproduces the target's separate base register held in $a0 across the whole function. */
void wldcore_window_move_status_help_cursor(wldcore_menu_status_help_level_t* request) {
    wldcore_window_xy_t* xy = g_wldcore_window_record_positions;
    s32 index = request->window_index;
    s32 field = request->field;
    s32 kind;

    xy[index].x = g_wldcore_status_help_fields[field].x + 14;
    xy[index].y = g_wldcore_status_help_fields[field].y + 12;
    kind = g_wldcore_status_help_fields[field].kind;
    if (kind != request->cached_field_kind) {
        request->cached_field_kind = kind;
        g_wldcore_window_records[index].sequence
            = (kind != 0) ? WLDCORE_ANIM_SEQUENCE_STATUS_HELP_CURSOR : WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
        g_wldcore_window_records[index].anim_counter = 0;
        g_wldcore_window_records[index].frame_index = 0;
    }
}
