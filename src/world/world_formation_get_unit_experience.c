#include "fft/world.h"

s32 world_formation_get_unit_experience(s32 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->experience;
}
