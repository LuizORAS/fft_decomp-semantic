#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * Event instruction: put the addressed unit(s) into their status idle
 * animation and wait.  Parameters: 0x00 unit id (16-bit value), 0x02 wait length
 * in frames.  A unit id above 0xff selects a group, so the misc-state loop
 * runs over all 21 unit indices.
 */
void world_script_march_units(const event_march_parameters_t* parameters) {
    u16 unit_id;
    s32 misc_index;
    s32 unit_index;

    unit_id = world_script_load_halfword(parameters->unit_id_le);
    if (world_unit_try_get_misc_data_by_unit_id(&unit_id, &misc_index) != 0) {
        unit_index = 0;
        do {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &misc_index) != 0) {
                battle_unit_set_status_animation_by_misc_id(unit_id);
                world_thread_wait_frames(parameters->wait_frames);
                if (misc_index == 0) {
                    break;
                }
            }
            unit_index++;
        } while (unit_index < BATTLE_UNIT_SLOT_COUNT);
    }
}
