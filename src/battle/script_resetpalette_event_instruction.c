#include "fft/battle_event.h"
#include "fft/battle_gfx.h"
#include "psx/types.h"

/* ResetPalette: resolve the unit id operand and reapply the weather, time-of-day
 * and tile palette modulation to that unit, unless the unit is missing. */
void script_resetpalette_event_instruction(const event_unit_parameters_t* parameters) {
    s32 misc_id = script_get_misc_id(script_load_halfword(parameters->unit_id_le));
    if (misc_id != EVENT_MISC_ID_NONE) {
        battle_gfx_apply_weather_time_tile_palette_mod_by_misc_id(misc_id);
    }
}
