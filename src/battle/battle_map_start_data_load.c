#include "fft/battle_map.h"
#include "psx/types.h"

/* Ask the map loader to load what the current map state needs. 1 when the load
 * started, 0 when a file read is still in progress. */
s32 battle_map_start_data_load(void) {
    return battle_map_load_data(g_battle_map_id, MAP_LOAD_COMMAND_START) != 0;
}
