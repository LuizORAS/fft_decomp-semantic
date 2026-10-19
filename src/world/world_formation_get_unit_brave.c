#include "fft/world.h"

/* Return formation unit `unit_id`'s Brave (menu script callback 6 of the unit cards). */
s32 world_formation_get_unit_brave(s32 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->identity.brave;
}
