#include "fft/event_require.h"
#include "psx/libgpu.h"
#include "psx/types.h"

void require_gfx_build_portrait_poly_ft4(s32 flags, POLY_FT4* output) {
    if ((flags & 0x300) == 0) {
        battle_menu_build_unit_portrait_poly(output, flags);
    }
    if ((flags & 0x100) != 0) {
        require_gfx_set_formation_icon_uv(output, flags & 0xff);
    }
}
