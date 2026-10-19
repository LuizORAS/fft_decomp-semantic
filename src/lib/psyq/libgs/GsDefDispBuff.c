#include "psx/libgs.h"
#include "psx/types.h"

/* Set the VRAM origins of the two display buffers, (`x0`, `y0`) and (`x1`, `y1`), and their drawing
 * offsets (zero in offset draw mode), then apply the active buffer's clip and offset.
 * wldcore_gfx_init_subsystem calls it. */
void GsDefDispBuff(u16 x0, u16 y0, u16 x1, u16 y1) {
    PSDOFSX[0] = x0;
    PSDOFSX[1] = x1;
    PSDOFSY[0] = y0;
    PSDOFSY[1] = y1;
    if (PSDGPU != 0) {
        PSDBASEX[0] = 0;
        PSDBASEX[1] = 0;
        PSDBASEY[0] = 0;
        PSDBASEY[1] = 0;
    } else {
        PSDBASEX[0] = x0;
        PSDBASEX[1] = x1;
        PSDBASEY[0] = y0;
        PSDBASEY[1] = y1;
    }
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}
