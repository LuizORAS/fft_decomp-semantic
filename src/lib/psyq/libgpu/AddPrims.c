/* LIBGPU 80023bf0-80023c2c. */
#include "psx/libgpu.h"

/* Link the chain from `first` to `last` at the head of the entry `ot`, as AddPrim does for one primitive.
 * battle_script_run_event_frame and world_script_run_event_frame insert their chains this way. */
void AddPrims(void* ot, void* first, void* last) {
    addPrims(ot, first, last);
}
