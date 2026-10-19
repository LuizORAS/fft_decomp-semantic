#include "fft/battle_unit.h"
#include "psx/types.h"

/* Same as battle_unit_update_display_by_runtime_id: the entry the event instructions and the staged status
 * routines call. */
void battle_unit_update_graphics_by_runtime_id(u32 runtime_id) {
    battle_unit_update_display_by_runtime_id(runtime_id);
}
