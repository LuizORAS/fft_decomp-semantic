#include "fft/battle.h"
#include "psx/types.h"

/* Return whether the map texture animation is still playing. */
s32 battle_map_command_get_texture_animation_active(s32 value) {
    return battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_GET_TEXTURE_ANIMATION_ACTIVE, value, 1, 1);
}
