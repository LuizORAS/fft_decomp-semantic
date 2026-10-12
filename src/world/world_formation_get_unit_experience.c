#include "fft/world.h"

/* Return formation unit `unit_id`'s EXP (menu script callback 5 of the unit cards). */
s32 world_formation_get_unit_experience(s32 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->experience;
}
