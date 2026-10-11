#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Draws the sprite described by a 9-byte command header (x, y, w, h, u, v)
 * and returns the next command. */
u8* world_menu_script_draw_sprite(u8* cmd) {
    world_menu_sprite_command_t* command = (world_menu_sprite_command_t*)cmd;
    RECT rect;
    s32 y;
    s16 step;
    rgb24_t* color;

    if (g_world_menu_use_scroll_position == 0) {
        y = command->sprite.y;
    } else {
        step = g_world_menu_row_height;
        y = step * g_world_menu_scroll_row_offset + command->sprite.y - g_world_menu_scroll_pixel_offset;
        if (g_world_menu_scroll_pixel_offset < 0) {
            y -= step;
        }
    }
    rect.x = command->sprite.x;
    rect.y = y;
    rect.w = command->sprite.w;
    rect.h = command->sprite.h;
    if (command->opcode == 4) {
        color = 0;
    } else {
        color = &g_world_menu_sprite_color;
    }
    world_gfx_enqueue_textured_quad(&rect, command->sprite.u, command->sprite.v, color, g_world_menu_semi_trans,
        g_world_menu_texture_page, g_world_menu_clut, g_world_menu_draw_priority);
    return (u8*)command + command->length;
}
