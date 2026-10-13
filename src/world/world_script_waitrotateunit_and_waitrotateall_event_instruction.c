#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* WaitRotateUnit / WaitRotateAll: yield until the named unit, or every unit
 * when the id is -1, has finished rotating. */
void world_script_waitrotateunit_and_waitrotateall_event_instruction(s32 unit_id) {
    s32 i;
    s32 runtime_id;

    if (unit_id == -1) {
        do {
            world_thread_yield();
            for (i = 0; i < EVENT_UNIT_SLOT_COUNT; i++) {
                if (g_world_unit_animation_states[i].rotating != 0) {
                    break;
                }
            }
        } while (i != EVENT_UNIT_SLOT_COUNT);
    } else {
        runtime_id = script_resolve_runtime_id(unit_id);
        if (runtime_id != EVENT_RUNTIME_ID_NONE) {
            do {
                world_thread_yield();
            } while (g_world_unit_animation_states[runtime_id].rotating != 0);
        }
    }
}
