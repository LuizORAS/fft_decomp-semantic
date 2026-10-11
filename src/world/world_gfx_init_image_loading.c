#include "fft/world.h"

/* Place a SPRT or POLY_FT4 from texture and screen parameters; BATTLE twin. */
void world_gfx_init_image_loading(world_texture_prim_t* primitive, const point16_t* base_load, const RECT* base_screen,
    const world_gfx_image_load_parameters_t* parameters) {
    int texture_column;

    if ((primitive->sprt.code & 0x7c) == 0x64) {
        if (parameters->x_load >= 0) {
            texture_column = base_load->x & 0x3f;
            primitive->sprt.u0 = (texture_column << 2) + parameters->x_load;
            primitive->sprt.v0 = base_load->y + parameters->y_load;
            primitive->sprt.w = parameters->width;
            primitive->sprt.h = parameters->height;
        }
        primitive->sprt.x0 = base_screen->x + parameters->x_screen_offset;
        primitive->sprt.y0 = base_screen->y + parameters->y_screen_offset;
        return;
    }

    if (parameters->x_load >= 0) {
        primitive->ft4.tpage = GetTPage(0, 0, base_load->x, base_load->y & 0xf00);
        primitive->ft4.u0 = parameters->x_load;
        primitive->ft4.v0 = base_load->y + parameters->y_load;
        primitive->ft4.u1 = parameters->x_load + parameters->width;
        primitive->ft4.v1 = base_load->y + parameters->y_load;
        primitive->ft4.u2 = parameters->x_load;
        primitive->ft4.v2 = base_load->y + parameters->y_load + parameters->height;
        primitive->ft4.u3 = parameters->x_load + parameters->width;
        primitive->ft4.v3 = base_load->y + parameters->y_load + parameters->height;
    }

    primitive->ft4.x0 = base_screen->x + parameters->x_screen_offset;
    primitive->ft4.y0 = base_screen->y + parameters->y_screen_offset;
    primitive->ft4.x1 = base_screen->x + parameters->x_screen_offset + parameters->width;
    primitive->ft4.y1 = base_screen->y + parameters->y_screen_offset;
    primitive->ft4.x2 = base_screen->x + parameters->x_screen_offset;
    primitive->ft4.y2 = base_screen->y + parameters->y_screen_offset + parameters->height;
    primitive->ft4.x3 = base_screen->x + parameters->x_screen_offset + parameters->width;
    primitive->ft4.y3 = base_screen->y + parameters->y_screen_offset + parameters->height;
}
