#include "fft/battle.h"
#include "psx/types.h"

/* Whether map texture animation `value` is still playing (map data command 0x82).
 * Defined void: battle_script_process_pending_requests reads the dispatcher's
 * result left in $v0. */
void battle_map_command_get_texture_animation_active(s32 value) {
    battle_map_dispatch_map_data_command(MAP_DATA_COMMAND_GET_TEXTURE_ANIMATION_ACTIVE, value, 1, 1);
}
