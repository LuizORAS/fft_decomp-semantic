#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of option_text_set_blit_source_rect: set the source rectangle of the frame-piece blit
 * (g_world_gfx_image_blit_source). */
void world_option_text_set_blit_source_rect(s16 x, s16 y, s16 width, s16 height) {
    g_world_gfx_image_blit_source.rect.x = x;
    g_world_gfx_image_blit_source.rect.y = y;
    g_world_gfx_image_blit_source.rect.w = width;
    g_world_gfx_image_blit_source.rect.h = height;
}
