#include "fft/battle_map.h"
#include "psx/types.h"

/* Start map texture animation `animation` from its first frame (map data command 0x83). `state` goes
 * in the object state argument, which that command does not read. Always returns 1. */
s32 battle_map_command_start_texture_animation(s32 animation, s32 state) {
    battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_START_TEXTURE_ANIMATION, animation, state, 1);
    return 1;
}
