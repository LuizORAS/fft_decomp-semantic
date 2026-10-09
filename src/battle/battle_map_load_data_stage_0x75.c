#include "fft/battle.h"
#include "psx/types.h"

s32 battle_map_load_data_stage_0x75(void) {
    return battle_map_load_data(g_battle_map_id, MAP_LOAD_COMMAND_START) != 0;
}
