#include "psx/libgs.h"
#include "psx/types.h"

void GsDefDispBuff(u16 x0, u16 y0, u16 x1, u16 y1) {
    PSDBASEX[0] = x0;
    PSDBASEX[1] = x1;
    PSDBASEY[0] = y0;
    PSDBASEY[1] = y1;
    if (g_psyq_gs_offset_draw_active != 0) {
        g_psyq_gs_buffer_offset_x[0] = 0;
        g_psyq_gs_buffer_offset_x[1] = 0;
        g_psyq_gs_buffer_offset_y[0] = 0;
        g_psyq_gs_buffer_offset_y[1] = 0;
    } else {
        g_psyq_gs_buffer_offset_x[0] = x0;
        g_psyq_gs_buffer_offset_x[1] = x1;
        g_psyq_gs_buffer_offset_y[0] = y0;
        g_psyq_gs_buffer_offset_y[1] = y1;
    }
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}
