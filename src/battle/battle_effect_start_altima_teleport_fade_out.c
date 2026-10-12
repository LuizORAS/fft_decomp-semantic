#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/map.h"

/* Teleport distortion: begin the arrival. Altima's forms release the ability
 * effect that battle_effect_start_altima_teleport_arrival requested; the unit's
 * palette is refreshed for the shadow mode of the tile it stands on and a palette
 * modulation of -31 on each colour component starts. The distortion moves to its
 * next phase with the timer at 0. */
void battle_effect_start_altima_teleport_fade_out(battle_unit_misc_data_t* unit) {
    map_tile_t* tile;

    if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM
        || unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM) {
        battle_effect_play();
    }
    tile = battle_map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
    battle_gfx_check_tile_status_palette_mod(unit, tile->flags.shading, 1);
    battle_gfx_start_misc_unit_palette_modulation(MAP_COLOR_MODE_BASE, 0, unit->unit_id, -31, -31, -31);
    unit->distortion_phase++;
    unit->distortion_timer = 0;
}
