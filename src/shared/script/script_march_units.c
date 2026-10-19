#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "psx/types.h"

/* March (EVENT_OPCODE_MARCH): put the unit, or each unit of a group in turn, into its status idle
 * animation and wait the operand's frames after each. Parameters: 0x00 unit id (16-bit value), 0x02 wait
 * length in frames. BATTLE and WORLD compile this text.
 *
 * A unit id above 0xff selects a group, so the runtime-state loop runs over all 21 unit indices. */
void script_march_units(const event_march_parameters_t* parameters) {
    u16 runtime_id;
    s32 state;
    s32 unit_index;

    runtime_id = script_load_halfword(parameters->unit_id_le);
    if (script_prepare_unit_filter(&runtime_id, &state) == 0) {
        return;
    }
    for (unit_index = 0; unit_index < EVENT_UNIT_SLOT_COUNT; unit_index++) {
        if (script_filter_unit_id_by_mode(&runtime_id, (u16*)&unit_index, &state) != 0) {
            battle_unit_set_status_animation_by_runtime_id(runtime_id);
            thread_wait_frames(parameters->wait_frames);
            if (state == 0) {
                return;
            }
        }
    }
}
