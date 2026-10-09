#include "fft/battle.h"
#include "psx/types.h"

void battle_script_waitrotateunit_and_waitrotateall_event_instruction(s32 unit_id) {
    s32 i;
    s32 index;

    if (unit_id == -1) {
        do {
            battle_thread_yield();
            for (i = 0; i < EVENT_UNIT_SLOT_COUNT; i++) {
                if (g_battle_unit_misc_rotation_data[i].rotating != 0) {
                    break;
                }
            }
        } while (i != EVENT_UNIT_SLOT_COUNT);
    } else {
        index = battle_script_get_misc_id(unit_id);
        if (index != EVENT_MISC_ID_NONE) {
            do {
                battle_thread_yield();
            } while (g_battle_unit_misc_rotation_data[index].rotating != 0);
        }
    }
}
