#include "fft/gfx.h"
#include "fft/world.h"
#include "psx/libgpu.h"

/* Set the text image's upload rectangle from its record (the VRAM origin, a
 * quarter of the inner width and the inner height) and set up its menu quad
 * through gfx_init_image_loading. */
void world_menu_init_quad_from_record(RECT* out, const world_menu_entry_t* record, SPRT* primitive) {
    out->x = record->vram_origin.x;
    out->y = record->vram_origin.y;
    out->w = record->inner_width >> 2;
    out->h = record->inner_height;
    world_menu_init_semitransparent_sprt(primitive);
    primitive->clut = 0x7C3C;
    gfx_init_image_loading(primitive, &record->vram_origin, &record->window, &record->text_image);
}
