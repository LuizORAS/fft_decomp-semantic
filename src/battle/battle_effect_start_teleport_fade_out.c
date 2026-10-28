#include "fft/battle_effect.h"
#include "fft/battle_gfx.h"
#include "fft/battle_map.h"
#include "fft/map.h"

/* Teleport step: play the pending effect (Altima's forms only), retint the unit for its tile, start
 * darkening its palette (-31), then advance distortion_phase and reset distortion_timer. */
void battle_effect_start_teleport_fade_out(battle_unit_t* unit) {
    map_tile_t* tile;

    if (unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_FIRST_FORM
        || unit->spritesheet_id == BATTLE_SPRITESHEET_ID_ALTIMA_SECOND_FORM) {
        battle_effect_play();
    }
    tile = battle_map_get_tile_data_pointer(unit->map_x, unit->map_y, unit->map_z);
    battle_gfx_check_tile_status_palette_mod(unit, tile->flags.shading, 1);
    battle_gfx_apply_unit_palette_modulation_to_units(MAP_COLOR_MODE_BASE, 0, unit->runtime_id, -31, -31, -31);
    unit->distortion_phase++;
    unit->distortion_timer = 0;
}
