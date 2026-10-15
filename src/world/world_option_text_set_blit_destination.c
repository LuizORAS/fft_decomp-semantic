#include "fft/world.h"
#include "psx/types.h"

/* WORLD's copy of option_text_set_blit_destination: set the destination origin of the frame-piece blit
 * (g_world_gfx_image_blit_destination). */
void world_option_text_set_blit_destination(s16 x, s16 y) {
    g_world_gfx_image_blit_destination.rect.x = x;
    g_world_gfx_image_blit_destination.rect.y = y;
}
