#include "fft/battle.h"
#include "psx/types.h"

/* Teleport distortion: begin the departure flash. Altima's forms release the
 * ability effect that battle_effect_start_altima_teleport_departure requested;
 * the sprite switches to additive semi-transparency and a palette modulation of
 * +31 on each colour component starts. The distortion moves to its next phase
 * with the timer at 0. */
void battle_effect_start_altima_teleport_white_flash(battle_unit_misc_data_t* unit) {
    if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM
        || unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM) {
        battle_effect_play();
    }
    unit->sprite_display_flags = (unit->sprite_display_flags & ~BATTLE_MISC_DISPLAY_BLEND_MASK)
        | (BATTLE_MISC_DISPLAY_BLEND_ADD | BATTLE_MISC_DISPLAY_SEMI_TRANSPARENT);
    battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_BASE, 2, unit->unit_id, 0x1f, 0x1f, 0x1f);
    unit->distortion_phase++;
    unit->distortion_timer = 0;
}
