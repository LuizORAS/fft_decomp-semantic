#include "fft/event_equip.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Texture a quad with a portrait: a unit portrait when neither bit 0x100 nor
 * 0x200 of `portrait_id` is set; otherwise the quad is left alone. */
void equip_gfx_build_portrait_poly_ft4(s32 portrait_id, void* poly) {
    if ((portrait_id & 0x300) == 0) {
        battle_menu_build_unit_portrait_poly(poly, portrait_id);
    }
}
