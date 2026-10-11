#include "fft/event_debugchr.h"
#include "psx/libgpu.h"

/* Texture a quad with a portrait: a unit portrait when neither bit 0x100 nor
 * 0x200 of `portrait_id` is set; otherwise the quad is left alone. */
void debugchr_gfx_build_portrait_poly_ft4(s32 portrait_id, POLY_FT4* poly) {
    if ((portrait_id & 0x300) == 0) {
        battle_menu_build_unit_portrait_poly(poly, portrait_id);
    }
}
