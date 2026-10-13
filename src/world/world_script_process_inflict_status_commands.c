#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"

/* Run staged status commands for units whose Float or Jump state needs a
 * dedicated update thread. */
void world_script_process_inflict_status_commands(void) {
    s32 instruction_offset;
    s32 runtime_id;
    s32 battle_id;
    s32 thread_id;
    const u8* instruction;
    unit_stats_t* unit;

    instruction_offset = 0;
    while ((instruction_offset
               = world_script_find_instruction_byte_offset(instruction_offset, EVENT_OPCODE_INFLICT_STATUS))
        != 0) {
        instruction = g_world_event_script;
        if (instruction[instruction_offset] == EVENT_OPCODE_INFLICT_STATUS) {
            runtime_id = script_resolve_runtime_id(script_read_s16(instruction + instruction_offset + 1));
            if (runtime_id != EVENT_RUNTIME_ID_NONE) {
                battle_id = battle_unit_get_battle_id_by_runtime_id(runtime_id);
                if (battle_id != 0x7d0) {
                    unit = battle_unit_get_stats_from_battle_id(battle_id);
                    if ((unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_FLOAT)]
                            & STATUS_BYTE_MASK(STATUS_ID_FLOAT))
                        || (unit->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_JUMP)]
                            & STATUS_BYTE_MASK(STATUS_ID_JUMP))) {
                        thread_id = world_thread_resolve_id(NATIVE_THREAD_SLOT_FIRST_IDLE);
                        thread_start(thread_id, world_script_inflict_status_thread);
                        thread_set_parameters(thread_id, (s32)(g_world_event_script + instruction_offset + 1), 0, 0);
                        thread_wait_until_inactive(thread_id);
                    }
                }
            }
        }
        instruction_offset += g_world_event_instruction_sizes[EVENT_OPCODE_INFLICT_STATUS] + 1;
    }
}
