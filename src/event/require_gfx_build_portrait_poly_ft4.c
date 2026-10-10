#include "fft/event_require.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Texture a quad with a portrait: a unit portrait when neither bit 0x100 nor
 * 0x200 of `flags` is set, and the formation portrait named by the low byte
 * when bit 0x100 is set. */
void require_gfx_build_portrait_poly_ft4(s32 flags, void* output) {
    if ((flags & 0x300) == 0) {
        battle_menu_build_unit_portrait_poly(output, flags);
    }
    if ((flags & 0x100) != 0) {
        require_gfx_set_formation_icon_uv(output, flags & 0xff);
    }
}
