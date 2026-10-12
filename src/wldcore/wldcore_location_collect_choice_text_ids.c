#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Collects the text ids of the location's selectable choices into out.
 *
 * Location WORLD_LOCATION_MUROND_DEATH_CITY returns -1, and locations whose script variable 0x267 + id is
 * set have no choices; otherwise the up-to-four entries of
 * g_wldcore_script_state.choice_vars are filtered by script variable
 * 0x292 + their menu byte, and at most five survive. */
s32 wldcore_location_collect_choice_text_ids(s32 location_id, s32* out) {
    s32 available;
    s32 i;
    s32 written;
    s32 entry;

    if (location_id == WORLD_LOCATION_MUROND_DEATH_CITY) {
        return -1;
    }
    if (script_get_variable(location_id + EVENT_SCRIPT_VAR_LOCATION_COLOR_FIRST) != 0) {
        return 0;
    }
    if (wldcore_script_process_conditional_set(g_wldcore_map_projection_state.marker.kind, WLDCORE_SCRIPT_ACTION_CHOICE)
        == 0) {
        return 0;
    }
    available = 0;
    for (i = 0; i < 4; i++) {
        if (g_wldcore_script_state.choice_vars[i] == 0) {
            break;
        }
        available++;
    }
    if (available == 1) {
        entry = g_wldcore_script_state.choice_vars[0];
        *out = entry + (TEXT_ID_MAP_NAME_BASE - 1);
        return 1;
    }
    written = 0;
    for (i = 0; i < available; i++) {
        entry = g_wldcore_script_state.choice_vars[i];
        if (script_get_variable(
                g_wldcore_choice_hidden_variable_offsets[entry] + EVENT_SCRIPT_VAR_LOCATION_CHOICE_HIDDEN_FIRST)
            == 0) {
            out[written++] = entry + (TEXT_ID_MAP_NAME_BASE - 1);
            if (written == 5) {
                break;
            }
        }
    }
    return written;
}
