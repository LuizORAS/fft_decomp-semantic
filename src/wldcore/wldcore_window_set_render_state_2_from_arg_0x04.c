#include "fft/wldcore.h"
#include "psx/types.h"

/* Give the render record named by the level's second word palette 2. */
void wldcore_window_set_render_state_2_from_arg_0x04(s32* param) {
    g_wldcore_window_render_records[param[1]].palette = 2;
}
