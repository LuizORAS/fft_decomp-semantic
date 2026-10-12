#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "psx/types.h"

/* Starts a unit's fade-out removal with a colour flash.
 *
 * The unit moves to the head of the misc list, enters BATTLE_MISC_FADE_OUT_FLASH,
 * loses its shadow and its float and depth offsets, and is drawn with additive
 * blending while its palette fades by (red, green, blue) from the last applied
 * colour. `mode` is the amount battle_map_modify_palette steps the fade by, not a
 * colour mode. battle_unit_update_fade_out_removal continues from there. */
void battle_gfx_configure_misc_unit_palette_modulation(s32 misc_id, s32 mode, s16 red, s16 green, s16 blue) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id);
    if (unit != 0) {
        battle_unit_move_misc_unit_to_head(misc_id);
        unit->fade_out_phase = BATTLE_MISC_FADE_OUT_FLASH;
        unit->shadow_graphic_trigger = 0;
        unit->sprite_display_flags = (unit->sprite_display_flags & ~BATTLE_MISC_DISPLAY_BLEND_MASK)
            | (BATTLE_MISC_DISPLAY_BLEND_ADD | BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT);
        battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_BASE, mode, misc_id, red, green, blue);
        unit->float_bob_offset = 0;
        unit->depth_height_offset = 0;
    }
}
