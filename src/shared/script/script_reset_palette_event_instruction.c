#include "fft/battle_gfx.h"
#include "fft/script.h"

/* ResetPalette: resolve the unit id operand and reapply the unit's weather and
 * time-of-day palette tint, with no tile effect level, unless the unit is
 * missing. */
void script_reset_palette_event_instruction(const event_unit_parameters_t* parameters) {
    s32 runtime_id = script_resolve_runtime_id(script_load_halfword(parameters->unit_id_le));
    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        battle_gfx_apply_weather_time_tile_palette_mod_by_runtime_id(runtime_id);
    }
}
