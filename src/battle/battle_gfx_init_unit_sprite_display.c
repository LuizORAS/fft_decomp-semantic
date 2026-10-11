#include "fft/battle.h"
#include "psx/types.h"

/* Reset a unit's own sprite display: neutral colour, no flags on its eight
 * parts and none of them in use. */
battle_gfx_sprite_display_data_t* battle_gfx_init_unit_sprite_display(s32 unit_index) {
    s32 body_part;
    battle_gfx_sprite_display_data_t* display;

    body_part = 7;
    display = &g_battle_unit_misc_data[unit_index].unit_sprite_display.header;
    display->blue = 0x80;
    display->green = 0x80;
    display->red = 0x80;
    do {
        display->parts[body_part].flags = 0;
        body_part--;
    } while (body_part >= 0);
    display->part_count = 0;
    return display;
}
