#include "fft/battle_map.h"
#include "psx/types.h"

/* Event Use3DObject: start animation state `state` of map mesh part `part` (map data command 0x80,
 * with flag 1). Always returns 1. */
s32 battle_map_command_set_3d_object_state(s32 part, s32 state) {
    battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_SET_3D_OBJECT_STATE, part, state, 1);
    return 1;
}
