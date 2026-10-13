#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Event instruction: put the addressed unit(s) into their status idle
 * animation and wait.  Parameters: 0x00 unit id (16-bit value), 0x02 wait length
 * in frames.  A unit id above 0xff selects a group, so the unit-state loop
 * runs over all 21 unit indices.
 */
void world_script_march_units(const event_march_parameters_t* parameters) {
    u16 unit_id;
    s32 filter_mode;
    s32 unit_index;

    unit_id = script_read_s16(parameters->unit_id_le);
    if (world_unit_prepare_event_filter(&unit_id, &filter_mode) != 0) {
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &filter_mode) != 0) {
                battle_unit_set_status_animation_by_runtime_id(unit_id);
                thread_wait_frames(parameters->wait_frames);
                if (filter_mode == 0) {
                    break;
                }
            }
        }
    }
}
