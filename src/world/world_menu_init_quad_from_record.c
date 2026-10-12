#include "fft/world.h"
#include "psx/libgpu.h"

/* Set the text image's upload rectangle and initialize its sprite. */
void world_menu_init_quad_from_record(RECT* out, const menu_entry_t* record, SPRT* primitive) {
    out->x = record->vram_origin.x;
    out->y = record->vram_origin.y;
    out->w = record->inner_width >> 2;
    out->h = record->inner_height;
    world_menu_init_sprite(primitive);
    primitive->clut = 0x7C3C;
    gfx_init_image_loading((texture_primitive_t*)primitive, &record->vram_origin, &record->window, &record->text_image);
}
