#include "fft/battle.h"

/* Build the window's background, edge sprites and optional compound-style sprites. */
void battle_menu_build_window_sprites(
    battle_menu_window_header_t* header, battle_menu_window_spec_t* spec, battle_menu_window_record_t* record) {
    s16 style;
    s32 index;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_MENU_WINDOW_BUILD);
    header->texture_x = spec->load.x;
    header->texture_y = spec->load.y;
    header->half_width = (s16)(spec->width) >> 2;
    header->height = spec->height;
    record->_unknown_74 = 0;
    record->_unknown_78 = 0;
    battle_menu_init_sprite_array(record->sprites, 4, 0x7cfc);
    battle_gfx_init_image_loading((POLY_FT4*)record->sprites, &spec->load, &spec->screen, &spec->params);
    SetSemiTrans(&record->sprites[1], 0);
    SetSemiTrans(&record->sprites[2], 1);
    record->sprites[1].w = 0x10;
    record->sprites[1].h = 0x10;
    record->sprites[2].w = 0x10;
    record->sprites[2].h = 0x10;
    record->sprites[1].u0 = 0xa8;
    record->sprites[1].v0 = 0;
    record->sprites[2].u0 = 0xb8;
    record->sprites[2].v0 = 0;
    style = spec->style;
    if (style < 5) {
        battle_gfx_init_image_loading((POLY_FT4*)&record->sprites[3], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_single_icon_image_params[style]);
    } else if (style >= 5 && style <= 15) {
        index = (style - 5) * 4;
        battle_menu_init_sprite_array(record->extra[0], 1, 0x7cfc);
        battle_menu_init_sprite_array(record->extra[1], 1, 0x7cfc);
        battle_menu_init_sprite_array(record->extra[2], 1, 0x7cfc);
        battle_gfx_init_image_loading((POLY_FT4*)&record->sprites[3], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_icon_strip_image_params[index]);
        battle_gfx_init_image_loading((POLY_FT4*)record->extra[0], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_icon_strip_image_params[index + 1]);
        battle_gfx_init_image_loading((POLY_FT4*)record->extra[1], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_icon_strip_image_params[index + 2]);
        battle_gfx_init_image_loading((POLY_FT4*)record->extra[2], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_icon_strip_image_params[index + 3]);
    } else {
        record->extra[0] = (SPRT*)0;
        record->extra[1] = (SPRT*)0;
        record->extra[2] = (SPRT*)0;
    }
    battle_gfx_set_draw_mode_from_rect(record, (const u16*)spec);
    SetDrawMode(&record->mode1, 0, 0, GetTPage(0, 2, 0x3c0, 0x100) & 0xffff, &g_battle_menu_disabled_texture_window);
}
