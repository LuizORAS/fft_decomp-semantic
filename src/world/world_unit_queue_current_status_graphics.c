#include "fft/battle_status.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Queue the renderer status flag change (`enabled`) for each of a unit's
 * current statuses, on the main stack. */
void world_unit_queue_current_status_graphics(s32 battle_unit_index, s32 enabled) {
    battle_stats_t* unit;
    s32 status_index;
    s32 byte_index;
    s32 bit_index;

    unit = battle_unit_get_stats_from_battle_id(battle_unit_index);
    g_thread_call_target = (void (*)(void))status_queue_runtime_graphics_flag_change;
    for (status_index = 0; status_index < STATUS_COUNT; status_index++) {
        byte_index = status_index / 8;
        bit_index = status_index - byte_index * 8;
        if ((unit->status_sets.current[byte_index] & (0x80 >> bit_index)) != 0) {
            thread_call_on_main_stack(STATUS_HANDLER_INDEX(status_index), enabled, battle_unit_index);
        }
    }
}
