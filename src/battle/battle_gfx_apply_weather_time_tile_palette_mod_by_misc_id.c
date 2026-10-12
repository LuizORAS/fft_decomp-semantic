#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Reloads a unit's palette with its weather and time-of-day tint and no tile
 * effect level, for the ResetPalette event instruction. An absent unit is
 * skipped. */
void battle_gfx_apply_weather_time_tile_palette_mod_by_misc_id(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id);

    if (unit != 0) {
        battle_gfx_tint_unit_palette_for_weather_and_tile(unit, 0, 1);
    }
}
