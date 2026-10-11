#include "fft/wldcore.h"

/* Resolve the script-selected location and focus the map on its window entry.
 */
void wldcore_opcode_map_focus_location(void) {
    u16 ip = g_wldcore_script_state.ip;
    u16* data = g_wldcore_script_state.data;
    u16 entry;
    s32 index;

    entry = data[ip];
    g_wldcore_script_state.args[0] = entry;
    if (entry == 0xff) {
        index = g_wldcore_map_projection_state.marker.kind;
        g_wldcore_proposition_selected_entry = index + 1;
    } else {
        index = entry;
        g_wldcore_proposition_selected_entry = index + 1;
    }
    if (wldcore_map_is_point_outside_projection_bounds(&g_wldcore_map_dots[index].render.position) != 0) {
        s32* flags = &g_wldcore_script_state.flags;

        *flags |= WLDCORE_SCRIPT_ACTION_FOCUS_LOCATION;
    }
    {
        s32* flags = &g_wldcore_script_state.flags;

        *flags |= WLDCORE_SCRIPT_RESULT_ACTION_REACHED;
    }
}
