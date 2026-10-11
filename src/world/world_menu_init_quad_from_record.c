#include "fft/world.h"
#include "psx/libgpu.h"

/* Set the text image's upload rectangle and initialize its sprite. */
void world_menu_init_quad_from_record(RECT* out, const world_menu_entry_t* record, SPRT* primitive) {
    out->x = record->vram_origin.x;
    out->y = record->vram_origin.y;
    out->w = record->inner_width >> 2;
    out->h = record->inner_height;
    world_menu_init_sprite(primitive);
    primitive->clut = 0x7C3C;
    world_gfx_init_image_loading(
        (world_texture_prim_t*)primitive, &record->vram_origin, &record->window, &record->text_image);
}
