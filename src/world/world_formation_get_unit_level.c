#include "fft/world.h"

s32 world_formation_get_unit_level(s32 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->level;
}
