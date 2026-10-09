#include "fft/world.h"
#include "psx/types.h"

s32 world_ability_is_selected_action_type(s32 unused_row) {
    return g_world_ability_selected_type == 0;
}
