#include "fft/battle.h"
#include "psx/types.h"

/* Draws a unit with the Transparent status semi-transparent with averaging blend
 * (the blend bits cleared), and any other unit opaque. */
void battle_gfx_update_sprite_transparency_flag(battle_unit_misc_data_t* unit) {
    u16 sprite_flags;

    if ((unit->status_flags_5_6 & BATTLE_MISC_STATUS_TRANSPARENT) != 0) {
        sprite_flags
            = (unit->sprite_display_flags & ~BATTLE_MISC_DISPLAY_BLEND_MASK) | BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT;
    } else {
        sprite_flags = unit->sprite_display_flags & ~BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT;
    }
    unit->sprite_display_flags = sprite_flags;
}
