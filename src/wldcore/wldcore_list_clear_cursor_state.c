#include "fft/wldcore.h"

/* Forget saved cursor slot `index`: row and scroll back to 0. */
void wldcore_list_clear_cursor_state(s32 index) {
    g_wldcore_menu_cursor_states[index].scroll_offset = 0;
    g_wldcore_menu_cursor_states[index].selected_index = 0;
}
