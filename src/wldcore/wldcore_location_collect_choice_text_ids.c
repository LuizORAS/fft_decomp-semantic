#include "fft/wldcore.h"
#include "psx/types.h"

/* The story event choices a red location offers when the marker arrives: the map-name text of each
 * choice of its WLDCORE_SCRIPT_ACTION_CHOICE condition (up to four), skipping those whose hidden
 * flag (EVENT_SCRIPT_VAR_LOCATION_CHOICE_HIDDEN_FIRST + the choice's offset) is set; a single choice
 * is never skipped. Returns how many, 0 for a blue or green location or no choice, and -1 for Murond
 * Death City. */
s32 wldcore_location_collect_choice_text_ids(s32 location_id, s32* out) {
    s32 available;
    s32 i;
    s32 written;
    s32 entry;

    if (location_id == WORLD_LOCATION_MUROND_DEATH_CITY) {
        return -1;
    }
    if (world_script_get_variable(location_id + EVENT_SCRIPT_VAR_LOCATION_COLOR_FIRST) != 0) {
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
        if (world_script_get_variable(
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
