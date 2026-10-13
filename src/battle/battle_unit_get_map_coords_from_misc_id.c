#include "fft/battle_unit.h"
#include "psx/types.h"

/* Writes a unit's tile as three 16-bit values in the order x, z, y and returns 1. A
 * misc id without a record raises pointer exception 12 and returns 0. */
s32 battle_unit_get_map_coords_from_misc_id(u32 misc_id, SVECTOR* coords) {
    battle_unit_t* unit = g_battle_unit_list_head;

    if (unit != 0) {
        do {
            if (unit->runtime_id == (misc_id & 0xffff)) {
                coords->vx = unit->map_x;
                coords->vz = unit->map_y;
                coords->vy = unit->map_z;
                return 1;
            }
            unit = unit->previous;
        } while (unit != 0);
    }
    main_system_handle_pointer_exception(12);
    return 0;
}
