#include "fft/world.h"
#include "psx/libgpu.h"

/* Set the text image's upload rectangle and initialize its sprite. */
void world_menu_init_quad_from_record(RECT* out, const world_menu_entry_t* record, SPRT* primitive) {
    out->x = record->vram_x;
    out->y = record->vram_y;
    out->w = record->inner_width >> 2;
    out->h = record->inner_height;
    world_menu_init_sprite(primitive);
    primitive->clut = 0x7C3C;
    world_gfx_init_image_loading(
        primitive, (const world_image_location_t*)record, (const RECT*)&record->window_x, &record->text_image);
}
