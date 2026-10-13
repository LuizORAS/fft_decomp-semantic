#include "fft/world.h"

s32 world_formation_get_unit_brave(s32 unit_id) {
    return g_world_unit_pointers[unit_id]->identity.brave;
}
