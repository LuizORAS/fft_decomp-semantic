#include "fft/battle_gfx.h"

/* Reset the display of one of a unit's three sprite blocks: neutral colour,
 * no flags on its three parts and none of them in use. */
battle_gfx_sprite_display_data_t* battle_gfx_init_weapon_sprite_display(s32 unit_index, s32 display_type) {
    s32 weapon_part;
    battle_gfx_sprite_display_data_t* display;
    battle_gfx_sprite_part_display_data_t* part;

    weapon_part = 2;
    display = &g_battle_unit_misc_data[unit_index].weapon_sprite_displays[display_type].header;
    part = display->parts;
    display->blue = 0x80;
    display->green = 0x80;
    display->red = 0x80;
    do {
        part[2].flags = 0;
        weapon_part--;
        part--;
    } while (weapon_part >= 0);
    display->part_count = 0;
    return display;
}
