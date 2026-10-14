#include "fft/battle.h"
#include "psx/types.h"

/* Animation state word of map mesh part `part` (map data command 0x81), which is 0 once the part's
 * animation has ended. Defined void: battle_script_process_pending_requests reads the dispatcher's result
 * left in $v0. */
void battle_map_command_get_3d_object_state(s32 part) {
    battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_GET_3D_OBJECT_STATE, part, 1, 1);
}
