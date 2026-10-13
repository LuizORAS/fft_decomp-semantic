#include "fft/battle_gfx.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/*
 * ColorUnit event instruction: start a palette modulation on the addressed
 * unit(s). Parameters: 0x00 unit id (16-bit value; a group id selects several
 * units), 0x02 mode, 0x03..0x05 signed red, green and blue amounts, 0x06
 * duration.
 */
void world_script_color_unit_event_instruction(const event_color_unit_parameters_t* parameters) {
    u16 unit_id;
    s32 misc_index;
    s32 unit_index;
    const event_unit_color_t* color;

    unit_id = script_read_s16(parameters->unit_id_le);
    if (world_unit_try_get_misc_data_by_unit_id(&unit_id, &misc_index) != 0) {
        color = &parameters->color;
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &misc_index) != 0) {
                battle_gfx_start_misc_unit_palette_modulation(
                    color->mode, color->duration, unit_id, color->rgb.r, color->rgb.g, color->rgb.b);
                if (misc_index == 0) {
                    break;
                }
            }
        }
    }
}
