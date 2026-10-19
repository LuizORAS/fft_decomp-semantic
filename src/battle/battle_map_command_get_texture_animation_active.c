#include "fft/battle_map.h"
#include "psx/types.h"

/* Whether map texture animation `animation` is still playing (map data command 0x82): the map data
 * dispatcher's result, which battle_script_process_pending_requests reads. */
s32 battle_map_command_get_texture_animation_active(s32 animation) {
    return battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_GET_TEXTURE_ANIMATION_ACTIVE, animation, 1, 1);
}
