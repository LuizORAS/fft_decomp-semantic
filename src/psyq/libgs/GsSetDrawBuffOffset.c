#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* Applies the current draw buffer's offsets. In offset draw mode the offset
 * moves into the draw environment and the GTE offsets are cleared; otherwise
 * the *other* buffer's offsets are programmed into the GTE and cached. */
void GsSetDrawBuffOffset(void) {
    s32 x;
    s32 y;

    if (g_psyq_gs_offset_draw_active != 0) {
        g_psyq_gs_offset_x = g_psyq_gs_offset_y = 0;
        GsDRAWENV.ofs[0] = POSITION.offx + PSDBASEX[PSDIDX];
        GsDRAWENV.ofs[1] = POSITION.offy + PSDBASEY[PSDIDX];
        PutDrawEnv(&GsDRAWENV);
    } else {
        x = POSITION.offx + PSDBASEX[PSDIDX != 0 ? 0 : 1];
        y = POSITION.offy + PSDBASEY[PSDIDX != 0 ? 0 : 1];
        SetGeomOffset(x, y);
        g_psyq_gs_offset_x = x;
        g_psyq_gs_offset_y = y;
    }
}
