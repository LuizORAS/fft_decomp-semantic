#include "fft/script.h"
#include "fft/wldcore.h"
#include "fft/world.h"
#include "psx/types.h"

/* Battle unit index of the displayed unit with the given event unit id, or
 * 2000 when there is none. */
s32 world_unit_get_battle_id_by_event_unit_id(s32 event_unit_id) {
    s32 runtime_id = script_resolve_runtime_id(event_unit_id);
    s32 unit_id;

    if (runtime_id == EVENT_RUNTIME_ID_NONE) {
        return 0x7D0;
    }
    unit_id = wldcore_unit_get_battle_id_by_runtime_id(runtime_id);
    if (unit_id != -1) {
        return unit_id;
    }
    unit_id = 0x7D0;
    return unit_id;
}
