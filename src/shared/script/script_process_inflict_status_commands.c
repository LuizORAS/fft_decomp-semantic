#include "fft/battle_event.h"
#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/unit.h"
#include "psx/types.h"

/* At the start of an event once the battle has begun (EVENT_SCRIPT_VAR_BATTLE_NOT_STARTED clear), run
 * each Inflict Status command (EVENT_OPCODE_INFLICT_STATUS) in the event block whose unit has Float or
 * Jump, each in its own thread (script_inflict_status_thread), waiting for it to end. The
 * WORLD twin is world_script_process_inflict_status_commands; the instruction scan and status-byte
 * layout match. */
void script_process_inflict_status_commands(void) {
    s32 instruction_offset;
    s32 runtime_id;
    s32 battle_id;
    s32 thread_id;
    const u8* instruction;
    battle_stats_t* unit;

    instruction_offset = 0;
    while ((instruction_offset = script_find_instruction_byte_offset(instruction_offset, EVENT_OPCODE_INFLICT_STATUS))
        != 0) {
        instruction = (const u8*)g_script_event_block;
        if (instruction[instruction_offset] == EVENT_OPCODE_INFLICT_STATUS) {
            runtime_id = script_resolve_runtime_id(script_load_halfword(instruction + instruction_offset + 1));
            if (runtime_id != EVENT_RUNTIME_ID_NONE) {
                battle_id = battle_unit_get_battle_id_by_runtime_id(runtime_id);
                if (battle_id != 0x7d0) {
                    unit = battle_unit_get_stats_from_battle_id(battle_id);
                    if ((unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_FLOAT)]
                            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_FLOAT))
                        || (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_JUMP)]
                            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_JUMP))) {
                        thread_id = battle_thread_resolve_id(THREAD_SLOT_FIRST_IDLE);
                        thread_start(thread_id, script_inflict_status_thread);
                        thread_set_parameters(
                            thread_id, (s32)((const u8*)g_script_event_block + instruction_offset + 1), 0, 0);
                        thread_wait_until_inactive(thread_id);
                    }
                }
            }
        }
        instruction_offset += g_script_event_instruction_sizes[EVENT_OPCODE_INFLICT_STATUS] + 1;
    }
}
