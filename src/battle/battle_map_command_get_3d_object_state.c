#include "fft/battle_map.h"
#include "psx/types.h"

/* Returns the 32-bit animation state of map mesh part `part` (map data command 0x81), which is 0
 * once the part's animation has ended: the map data dispatcher's result, which
 * battle_script_process_pending_requests reads. */
s32 battle_map_command_get_3d_object_state(s32 part) {
    return battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_GET_3D_OBJECT_STATE, part, 1, 1);
}
