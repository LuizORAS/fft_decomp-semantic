#include "fft/battle.h"

battle_gfx_sprite_display_data_t* battle_gfx_init_item_sprite_display(s32 unit_index) {
    battle_gfx_sprite_display_data_t* display;
    s32 i;

    display = &g_battle_unit_misc_data[unit_index].item_sprite_display;
    display->blue = 0x80;
    display->green = 0x80;
    display->red = 0x80;
    display->scale_y = ONE;
    display->scale_x = ONE;
    display->tpage = BATTLE_GFX_TPAGE_ITEM;
    display->clut = MAP_CLUT(MAP_CLUT_ROW_SPRITES, 7);
    display->rotation = 0;
    display->part_count = 1;
    for (i = 0; i < 1; i++) {
        display->parts[i].y_shift = 0;
        display->parts[i].x_shift = 0;
        display->parts[i].width = 0x10;
        display->parts[i].height = 0x10;
        display->parts[i].flags = 0;
    }
    return display;
}
