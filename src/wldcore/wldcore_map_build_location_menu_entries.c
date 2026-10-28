#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* The entries a location's menu offers, written to `out`; returns how many. The Deep Dungeon offers
 * its floors (one more than EVENT_SCRIPT_VAR_DEEP_DUNGEON_LIST_LENGTH); a town (its record's is_town
 * is 1) offers the Bar, the Shop and the Soldier Office, plus the Fur Shop at Dorter, Warjilis and
 * Zarghidas once EVENT_SCRIPT_VAR_FUR_SHOP_ENABLED is set; any other location offers none. */
s32 wldcore_map_build_location_menu_entries(s32 location_id, s32* out) {
    s32 count;
    s32 i;

    if (location_id == WORLD_LOCATION_DEEP_DUNGEON) {
        count = script_get_variable(EVENT_SCRIPT_VAR_DEEP_DUNGEON_LIST_LENGTH) + 1;
        for (i = 0; i < count; i++) {
            out[i] = WLDCORE_TEXT_ID_DEEP_DUNGEON_FLOOR_FIRST + i;
        }
        return count;
    }

    count = 0;
    if (g_wldcore_location_records[location_id].is_town == 1) {
        count = 3;
        out[0] = WLDCORE_TEXT_ID_BAR;
        out[1] = WLDCORE_TEXT_ID_SHOP;
        out[2] = WLDCORE_TEXT_ID_SOLDIER_OFFICE;
        if (script_get_variable(EVENT_SCRIPT_VAR_FUR_SHOP_ENABLED) != 0) {
            if ((location_id == WORLD_LOCATION_DORTER) || (location_id == WORLD_LOCATION_WARJILIS)
                || (location_id == WORLD_LOCATION_ZARGHIDAS)) {
                out[count] = WLDCORE_TEXT_ID_FUR_SHOP;
                count++;
            }
        }
    }
    return count;
}
