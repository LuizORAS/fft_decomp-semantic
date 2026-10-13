#include "fft/battle_script.h"
#include "fft/battle_thread.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* WaitRotateUnit and WaitRotateAll: yield until the named unit, or every unit
 * unit when the id is -1, has finished rotating. Both forms yield at least once;
 * a unit id that does not resolve returns at once. */
void script_waitrotateunit_and_waitrotateall_event_instruction(s32 unit_id) {
    s32 i;
    s32 index;

    if (unit_id == -1) {
        do {
            thread_yield();
            for (i = 0; i < EVENT_UNIT_SLOT_COUNT; i++) {
                if (g_battle_unit_animation_states[i].rotating != 0) {
                    break;
                }
            }
        } while (i != EVENT_UNIT_SLOT_COUNT);
    } else {
        index = script_resolve_runtime_id(unit_id);
        if (index != EVENT_RUNTIME_ID_NONE) {
            do {
                thread_yield();
            } while (g_battle_unit_animation_states[index].rotating != 0);
        }
    }
}
