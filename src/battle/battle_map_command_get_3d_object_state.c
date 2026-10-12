#include "fft/battle_map.h"
#include "psx/types.h"

/* Return the map mesh part's animation state; zero means its animation has ended. */
s32 battle_map_command_get_3d_object_state(s32 value) {
    return battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_GET_3D_OBJECT_STATE, value, 1, 1);
}
