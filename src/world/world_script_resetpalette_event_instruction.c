#include "fft/battle_gfx.h"
#include "fft/script.h"
#include "fft/world.h"

/* Resolve the script's signed unit ID and refresh its environment palette
 * unless resolution returns the missing-unit sentinel. */
void world_script_resetpalette_event_instruction(const event_unit_parameters_t* parameters) {
    s32 runtime_id = script_resolve_runtime_id(script_read_s16(parameters->unit_id_le));

    if (runtime_id != EVENT_RUNTIME_ID_NONE) {
        battle_gfx_apply_weather_time_tile_palette_mod_by_runtime_id(runtime_id);
    }
}
