#include "fft/world.h"

/* Return formation unit `unit_id`'s Faith (menu script callback 7 of the unit cards). */
s32 world_formation_get_unit_faith(s32 unit_id) {
    return g_world_formation_unit_pointers[unit_id]->faith;
}
