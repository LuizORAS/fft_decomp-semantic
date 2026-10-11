#include "fft/battle.h"

/* Resets one of a unit's three numeric sprite displays to neutral colour and no
 * parts. A display_section outside 0..2 leaves `display` unset; the only caller
 * passes 0, 1 and 2. */
battle_gfx_sprite_display_data_t* battle_gfx_init_numeric_sprite_display(s32 unit_index, s32 display_section) {
    battle_gfx_sprite_display_data_t* display;
    s32 i;

    switch (display_section) {
    case 0:
        display = &g_battle_unit_misc_data[unit_index].numeric_sprite_displays[0];
        break;
    case 1:
        display = &g_battle_unit_misc_data[unit_index].numeric_sprite_displays[1];
        break;
    case 2:
        display = &g_battle_unit_misc_data[unit_index].numeric_sprite_displays[2];
        break;
    }

    display->blue = 0x80;
    display->green = 0x80;
    display->red = 0x80;
    for (i = 0; i < 1; i++)
        display->parts[i].flags = 0;
    display->part_count = 0;
    return display;
}
