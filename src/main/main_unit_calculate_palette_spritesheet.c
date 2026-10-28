#include "fft/main.h"

/* Store the unit's spritesheet and palette (main_unit_get_spritesheet_palette) in spritesheet_id and
 * job_portrait_palette; returns the spritesheet, with the palette in *palette. */
u8 main_unit_calculate_palette_spritesheet(battle_stats_t* unit, u8* palette) {
    u8 spritesheet = main_unit_get_spritesheet_palette(unit, palette);

    unit->spritesheet_id = spritesheet;
    unit->job_portrait_palette = *palette;
    return spritesheet;
}
