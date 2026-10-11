#include "fft/wldcore.h"
#include "psx/types.h"

/* Show the owner's window (a "more" marker) while pages remain after the
 * current one; hide it on the last page. */
void wldcore_window_update_flag_from_owner_progress(wldcore_window_owner_t* owner) {
    if (owner->page_start_row < owner->last_page_row) {
        g_wldcore_window_records[owner->window_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    } else {
        g_wldcore_window_records[owner->window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
}
