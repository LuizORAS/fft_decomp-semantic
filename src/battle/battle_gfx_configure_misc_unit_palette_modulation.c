#include "fft/battle.h"
#include "psx/types.h"

void battle_gfx_configure_misc_unit_palette_modulation(s32 misc_id, s32 mode, s16 red, s16 green, s16 blue) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        battle_unit_move_misc_unit_to_head(misc_id);
        unit->fade_out_phase = BATTLE_MISC_FADE_OUT_FLASH;
        unit->shadow_graphic_trigger = 0;
        unit->sprite_display_flags.half = (unit->sprite_display_flags.half & ~BATTLE_MISC_DISPLAY_BLEND_MASK)
            | (BATTLE_MISC_DISPLAY_BLEND_ADD | BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT);
        battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_BASE, mode, misc_id, red, green, blue);
        unit->float_bob_offset = 0;
        unit->depth_height_offset = 0;
    }
}
