#include "fft/wldcore.h"

/* Suspend handler of a proposition's detail (level PROPOSITION_DETAIL): its text render record (the level's second
 * 32-bit value) takes CLUT 2. wldcore_proposition_handle_accept_input and wldcore_proposition_step_send_unit_level call
 * it before they open a level over the detail. */
void wldcore_menu_suspend_proposition_detail_level(wldcore_menu_window_render_level_t* pair) {
    g_wldcore_window_render_records[pair->render_index].palette = 2;
}
