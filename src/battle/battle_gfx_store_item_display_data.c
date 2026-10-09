#include "fft/battle.h"

typedef struct battle_gfx_item_graphic_data {
    u8 _unused_00[0xc];
    u8 x_load_location;
    u8 y_load_location;
    u16 clut;
    s16 width;
    s16 height;
} battle_gfx_item_graphic_data_t;

void battle_gfx_store_item_display_data(battle_gfx_render_unit_t* unit, u32 item_id) {
    battle_gfx_item_graphic_data_t data;

    battle_gfx_set_item_icon_sprite((SPRT*)&data, item_id);
    unit->found_item_display->clut = data.clut;
    battle_gfx_store_sprite_display_data(unit->found_item_display, 0, (s16)((16 - data.width) / 2 - 8),
        (s16)((16 - data.height) / 2 - 8), data.x_load_location, data.y_load_location, data.width, data.height, 0);
}
