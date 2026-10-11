#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Place a cursor sprite over the icon strip: position from the icon origin,
 * texture origin from the text image's VRAM position ((x & 0x3f) * 4 and the
 * low byte of y), size from the text sprite placement. */
void world_gfx_copy_sprite_position_uv_and_size(const world_menu_entry_t* param, SPRT* sprite, s32 unused_frame) {
    sprite->x0 = param->window_x;
    sprite->y0 = param->window_y;
    sprite->u0 = (param->vram_x & 0x3F) * 4;
    sprite->v0 = param->vram_y;
    sprite->w = param->text_image.width;
    sprite->h = param->text_image.height;
}
