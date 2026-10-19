#include "fft/world.h"

/* Days an egg has been incubating, for its card (menu script callback 2): its birthday packs the
 * days from laying to hatching above the low nibble and the days left in it (counted down by
 * world_unit_update_monster_breeding), and this returns the first minus the second. */
s32 world_formation_get_unit_egg_hatch_progress(s32 unit_id) {
    world_unit_t* unit = g_world_formation_unit_pointers[unit_id];
    s32 low = unit->birthday & 0xF;
    return (unit->birthday >> 4) - low;
}
