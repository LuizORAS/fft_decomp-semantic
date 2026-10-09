#include "fft/battle.h"
#include "psx/types.h"

void battle_script_resetpalette_event_instruction(const u8* parameters) {
    s32 misc_id = battle_script_get_misc_id(battle_script_load_halfword(parameters));
    if (misc_id != EVENT_MISC_ID_NONE) {
        battle_gfx_apply_weather_time_tile_palette_mod_by_misc_id(misc_id);
    }
}
