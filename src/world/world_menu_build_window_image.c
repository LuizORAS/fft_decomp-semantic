#include "fft/world.h"
#include "psx/libgpu.h"

/* WORLD twin of battle_menu_build_window_image. */
void* world_menu_build_window_image(s32 width, s32 height, RECT* rect, s32 mode, s32 tail_offset, s32 keep) {
    u16 kind = mode & DIALOG_TYPE_OPTIONS_MASK;
    s32 words;
    s32 partial;
    s32 byte_count;
    u16* image;
    u16* sheet;
    s32 x_offset;
    s32 i;
    s32 sheet_offset;
    u16* row;

    words = width & 3;
    partial = words != 0;
    words = (width >> 2) + partial;
    byte_count = (width * (kind == DIALOG_OPTIONS_NO_TAIL ? height : height + 0x10)) >> 1;
    image = world_menu_alloc_memory(byte_count);
    world_clear_menu_render_buffer((u8*)image, byte_count);
    x_offset = 0;
    sheet = (u16*)g_world_menu_glyph_image;
    if (kind != DIALOG_OPTIONS_NO_TAIL) {
        if ((mode & DIALOG_TYPE_POSITION_MASK) == 2) {
            x_offset = words * 8;
        }
        if ((mode & DIALOG_TYPE_OPTIONS_MASK) == DIALOG_OPTIONS_TAIL) {
            height -= 8;
        }
        if ((mode & DIALOG_TYPE_OPTIONS_MASK) == DIALOG_OPTIONS_ALTERNATE_TAIL) {
            height -= 0x10;
        }
    }
    for (i = 0; i < height; i++) {
        if (i < 8) {
            sheet_offset = (i << 6) + 0xA;
        } else if (i >= height - 8) {
            sheet_offset = (((i - height + 8) & 7) << 6) + 0x60A;
        } else {
            sheet_offset = ((((i - 8) & 0xF) + 8) << 6) + 0xA;
        }
        row = image + i * words + x_offset;
        row[words - 1] = 0;
        row[words - 2] = 0;
        world_menu_build_window_image_row(sheet, row, words, sheet_offset);
    }
    if (kind != DIALOG_OPTIONS_NO_TAIL) {
        g_world_menu_window_sprite_glyph.rect.x = 0x58;
        g_world_menu_window_sprite_glyph.rect.y = 0;
        if ((mode & DIALOG_TYPE_OPTIONS_MASK) == DIALOG_OPTIONS_ALTERNATE_TAIL) {
            g_world_menu_window_sprite_glyph.rect.x = 0x68;
        }
        if ((mode & DIALOG_TYPE_POSITION_MASK) == 1) {
            g_world_menu_window_sprite_glyph.rect.y = 0x10;
        }
        g_world_menu_window_sprite_position.rect.x = (width >> 1) + tail_offset - 8;
        if ((mode & DIALOG_TYPE_POSITION_MASK) != 2) {
            g_world_menu_window_sprite_position.rect.y = 0x38;
        } else {
            g_world_menu_window_sprite_position.rect.y = 0;
        }
        g_world_menu_window_sprite_position.stride = words * 4;
        i = g_world_threads[g_world_thread_current_id].task_words[5] & 0xF0;
        if (i != 0) {
            if ((mode & DIALOG_TYPE_POSITION_MASK) == 1) {
                g_world_menu_window_sprite_position.rect.x -= 0x10;
            } else {
                g_world_menu_window_sprite_position.rect.x += 0x10;
            }
        }
        if (g_world_menu_window_sprite_position.rect.x < 0x10) {
            g_world_menu_window_sprite_position.rect.x = 0x10;
        }
        if (width - 0x10 < g_world_menu_window_sprite_position.rect.x) {
            g_world_menu_window_sprite_position.rect.x = width - 0x10;
        }
        world_text_blit_glyph(
            g_world_menu_glyph_image, image, &g_world_menu_window_sprite_glyph, &g_world_menu_window_sprite_position);
        if (i != 0) {
            world_gfx_mirror_4bpp_rect((u8*)image, &g_world_menu_window_sprite_position);
        }
    }
    if (keep != 0) {
        return image;
    }
    LoadImage(rect, (u32*)image);
    world_thread_wait_frames(1);
    world_menu_free_memory(image);
}
