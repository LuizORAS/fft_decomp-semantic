#include "fft/battle_move.h"
#include "psx/types.h"

/* Return whether the unit still has a path to walk (0 and pointer exception 12 for a missing
 * unit). */
s32 battle_move_is_unit_moving_by_runtime_id(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    if (unit == 0) {
        main_system_handle_pointer_exception(12);
        return 0;
    }
    return unit->path.count != 0;
}
