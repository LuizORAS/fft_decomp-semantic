#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Battle twin of world_gfx_init_image_loading: fill a textured quad from load and screen placement parameters. */
void battle_gfx_init_image_loading(void* prim, const world_image_location_t* base_load, const RECT* base_screen,
    const world_gfx_image_load_parameters_t* parameters) {
    POLY_FT4* primitive = prim;
    int texture_column;

    if ((primitive->code & 0x7c) == 0x64) {
        if (parameters->x_load >= 0) {
            texture_column = base_load->x & 0x3f;
            primitive->u0 = (texture_column << 2) + parameters->x_load;
            primitive->v0 = base_load->y + parameters->y_load;
            primitive->x1 = parameters->width;
            primitive->y1 = parameters->height;
        }
        primitive->x0 = base_screen->x + parameters->x_screen_offset;
        primitive->y0 = base_screen->y + parameters->y_screen_offset;
        return;
    }

    if (parameters->x_load >= 0) {
        primitive->tpage = GetTPage(0, 0, base_load->x, base_load->y & 0xf00);
        primitive->u0 = parameters->x_load;
        primitive->v0 = base_load->y + parameters->y_load;
        primitive->u1 = parameters->x_load + parameters->width;
        primitive->v1 = base_load->y + parameters->y_load;
        primitive->u2 = parameters->x_load;
        primitive->v2 = base_load->y + parameters->y_load + parameters->height;
        primitive->u3 = parameters->x_load + parameters->width;
        primitive->v3 = base_load->y + parameters->y_load + parameters->height;
    }

    primitive->x0 = base_screen->x + parameters->x_screen_offset;
    primitive->y0 = base_screen->y + parameters->y_screen_offset;
    primitive->x1 = base_screen->x + parameters->x_screen_offset + parameters->width;
    primitive->y1 = base_screen->y + parameters->y_screen_offset;
    primitive->x2 = base_screen->x + parameters->x_screen_offset;
    primitive->y2 = base_screen->y + parameters->y_screen_offset + parameters->height;
    primitive->x3 = base_screen->x + parameters->x_screen_offset + parameters->width;
    primitive->y3 = base_screen->y + parameters->y_screen_offset + parameters->height;
}
