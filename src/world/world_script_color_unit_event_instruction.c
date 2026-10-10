#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * ColorUnit event instruction: start a palette modulation on the addressed
 * unit(s). Parameters: 0x00 unit id (halfword; a group id selects several
 * units), 0x02 mode, 0x03..0x05 signed red, green and blue amounts, 0x06
 * duration.
 */
void world_script_color_unit_event_instruction(const u8* parameters) {
    u16 unit_id;
    s32 misc_index;
    s32 unit_index;

    unit_id = world_script_load_halfword(parameters);
    if (world_unit_try_get_misc_data_by_unit_id(&unit_id, &misc_index) != 0) {
        parameters += 2;
        unit_index = 0;
        do {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &misc_index) != 0) {
                battle_gfx_start_misc_unit_palette_modulation(
                    parameters[0], parameters[4], unit_id, (s8)parameters[1], (s8)parameters[2], (s8)parameters[3]);
                if (misc_index == 0) {
                    break;
                }
            }
            unit_index++;
        } while (unit_index < BATTLE_UNIT_SLOT_COUNT);
    }
}
