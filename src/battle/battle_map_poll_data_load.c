#include "fft/battle_map.h"
#include "psx/types.h"

/* Advance the current map's load by one step. Nonzero while it is still loading. */
s32 battle_map_poll_data_load(void) {
    return battle_map_load_data(g_battle_map_id, MAP_LOAD_COMMAND_POLL);
}
