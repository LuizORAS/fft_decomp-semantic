#include "fft/battle_unit.h"
#include "fft/main.h"
#include "psx/types.h"

/* The same routine as battle_unit_add_event_offset_by_runtime_id, compiled twice. Nothing calls this copy. */
s32 battle_unit_add_event_offset_by_runtime_id_unused(u32 runtime_id, const SVECTOR* amount) {
    battle_unit_t* unit;

    unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);
    if (unit != 0) {
        unit->event_offset.vx = unit->event_offset.vx + amount->vx;
        unit->event_offset.vy = unit->event_offset.vy + amount->vy;
        unit->event_offset.vz = unit->event_offset.vz + amount->vz;
    } else {
        main_system_handle_pointer_exception(12);
        return 0;
    }
    return 1;
}
