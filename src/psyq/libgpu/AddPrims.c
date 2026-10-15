/* LIBGPU 80023bf0-80023c2c. */
#include "psx/libgpu.h"

/* Link the chain from `first` to `last` at the head of the entry `ot`, as AddPrim does for one primitive.
 * battle_script_run_event_frame and world_script_run_event_frame insert their chains this way. */
void AddPrims(u32* ot, void* first, void* last) {
    ((P_TAG*)last)->addr = ((P_TAG*)ot)->addr;
    ((P_TAG*)ot)->addr = (u32)first;
}
