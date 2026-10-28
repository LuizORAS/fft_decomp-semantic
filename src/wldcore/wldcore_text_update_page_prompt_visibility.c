#include "fft/wldcore.h"

/* Show the owner's window (a "more" marker) while pages remain after the
 * current one; hide it on the last page. */
void wldcore_text_update_page_prompt_visibility(wldcore_text_scrollable_window_t* state) {
    if (state->page_start_row < state->last_page_row) {
        g_wldcore_window_records[state->left_window_index].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
    } else {
        g_wldcore_window_records[state->left_window_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    }
}
