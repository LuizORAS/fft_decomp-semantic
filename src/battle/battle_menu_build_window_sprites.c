#include "fft/battle.h"

/* Build the window's background, edge sprites and optional compound-style sprites. */
void battle_menu_build_window_sprites(
    battle_menu_window_header_t* header, battle_menu_window_spec_t* spec, world_menu_icon_record_t* record) {
    s16 style;
    s32 index;

    battle_thread_set_current_task_id(NATIVE_THREAD_TASK_MENU_WINDOW_BUILD);
    header->texture_x = spec->load.x;
    header->texture_y = spec->load.y;
    header->half_width = (s16)(spec->width) >> 2;
    header->height = spec->height;
    record->_unknown_74 = 0;
    record->_unknown_78 = 0;
    battle_menu_init_sprite_array(record->base.sprites, 4, 0x7cfc);
    battle_gfx_init_image_loading((POLY_FT4*)record->base.sprites, &spec->load, &spec->screen, &spec->params);
    SetSemiTrans(&record->base.sprites[1], 0);
    SetSemiTrans(&record->base.sprites[2], 1);
    record->base.sprites[1].w = 0x10;
    record->base.sprites[1].h = 0x10;
    record->base.sprites[2].w = 0x10;
    record->base.sprites[2].h = 0x10;
    record->base.sprites[1].u0 = 0xa8;
    record->base.sprites[1].v0 = 0;
    record->base.sprites[2].u0 = 0xb8;
    record->base.sprites[2].v0 = 0;
    style = spec->style;
    if (style < 5) {
        battle_gfx_init_image_loading((POLY_FT4*)&record->base.sprites[3], &g_battle_menu_texture_location,
            &spec->screen, &g_battle_menu_single_icon_image_params[style]);
    } else if (style >= 5 && style <= 15) {
        index = (style - 5) * 4;
        battle_menu_init_sprite_array(record->icons[0], 1, 0x7cfc);
        battle_menu_init_sprite_array(record->icons[1], 1, 0x7cfc);
        battle_menu_init_sprite_array(record->icons[2], 1, 0x7cfc);
        battle_gfx_init_image_loading((POLY_FT4*)&record->base.sprites[3], &g_battle_menu_texture_location,
            &spec->screen, &g_battle_menu_icon_strip_image_params[index]);
        battle_gfx_init_image_loading((POLY_FT4*)record->icons[0], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_icon_strip_image_params[index + 1]);
        battle_gfx_init_image_loading((POLY_FT4*)record->icons[1], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_icon_strip_image_params[index + 2]);
        battle_gfx_init_image_loading((POLY_FT4*)record->icons[2], &g_battle_menu_texture_location, &spec->screen,
            &g_battle_menu_icon_strip_image_params[index + 3]);
    } else {
        record->icons[0] = (SPRT*)0;
        record->icons[1] = (SPRT*)0;
        record->icons[2] = (SPRT*)0;
    }
    battle_gfx_set_draw_mode_from_rect(record, (const u16*)spec);
    SetDrawMode(
        &record->base.draw_mode, 0, 0, GetTPage(0, 2, 0x3c0, 0x100) & 0xffff, &g_battle_menu_disabled_texture_window);
}
