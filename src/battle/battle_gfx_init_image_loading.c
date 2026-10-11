#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Place a SPRT or POLY_FT4 from texture and screen parameters; WORLD twin. */
void battle_gfx_init_image_loading(texture_primitive_t* primitive, const point16_t* base_load, const RECT* base_screen,
    const image_load_parameters_t* parameters) {
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
        setUV4(&primitive->ft4, parameters->x_load, base_load->y + parameters->y_load,
            parameters->x_load + parameters->width, base_load->y + parameters->y_load, parameters->x_load,
            base_load->y + parameters->y_load + parameters->height, parameters->x_load + parameters->width,
            base_load->y + parameters->y_load + parameters->height);
    }

    setXY4(&primitive->ft4, base_screen->x + parameters->x_screen_offset, base_screen->y + parameters->y_screen_offset,
        base_screen->x + parameters->x_screen_offset + parameters->width, base_screen->y + parameters->y_screen_offset,
        base_screen->x + parameters->x_screen_offset, base_screen->y + parameters->y_screen_offset + parameters->height,
        base_screen->x + parameters->x_screen_offset + parameters->width,
        base_screen->y + parameters->y_screen_offset + parameters->height);
}
