#include "fft/battle_gfx.h"
#include "fft/battle_script.h"
#include "fft/script.h"
#include "psx/types.h"

/* ResetPalette: resolve the unit id operand and reapply the weather, time-of-day
 * and tile palette modulation to that unit, unless the unit is missing. */
void script_resetpalette_event_instruction(const event_unit_parameters_t* parameters) {
    s32 runtime_id = script_resolve_runtime_id(script_read_s16(parameters->unit_id_le));
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        battle_gfx_apply_weather_time_tile_palette_mod_by_runtime_id(runtime_id);
    }
}
