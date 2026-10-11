#include "fft/world.h"

/* The bits above the low nibble of an egg's birthday value (birthday >> 4). */
s32 world_formation_get_unit_egg_hatch_total(s32 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->birthday >> 4;
}
