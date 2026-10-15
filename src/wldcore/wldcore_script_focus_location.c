#include "fft/wldcore.h"

/* Location script opcode 36: end the script with the camera focused on a location (0xff: the
 * current one), which the location label then names; the focus is requested only when the location's
 * dot lies outside the map view. */
void wldcore_script_focus_location(void) {
    u16 ip = g_wldcore_script_state.ip;
    u16* data = g_wldcore_script_state.data;
    u16 entry;
    s32 index;

    entry = data[ip];
    g_wldcore_script_state.args[0] = entry;
    if (entry == 0xff) {
        index = g_wldcore_map_projection_state.marker.kind;
        g_wldcore_map_label_location = index + 1;
    } else {
        index = entry;
        g_wldcore_map_label_location = index + 1;
    }
    if (wldcore_map_is_point_outside_projection_bounds((const wldcore_point32_t*)&g_wldcore_map_dot_screen_x[index])
        != 0) {
        s32* flags = &g_wldcore_script_state.flags;

        *flags |= WLDCORE_SCRIPT_ACTION_FOCUS_LOCATION;
    }
    {
        s32* flags = &g_wldcore_script_state.flags;

        *flags |= WLDCORE_SCRIPT_RESULT_ACTION_REACHED;
    }
}
