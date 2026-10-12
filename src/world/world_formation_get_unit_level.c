#include "fft/world.h"

/* Return formation unit `unit_id`'s level (menu script callback 4 of the unit cards,
 * world_formation_init_selection_cursor). */
s32 world_formation_get_unit_level(s32 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->level;
}
