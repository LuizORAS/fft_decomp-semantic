#include "fft/event_equip.h"
#include "psx/types.h"

/* `rect` carries the two rect as (x0, y0, x1, y1). */
void equip_gfx_enqueue_line_f2(const RECT* rect, const rgb24_t* color, u8 semitrans, s32 otag_index) {
    LINE_F2* line;

    line = &g_equip_gfx_context->lines[g_equip_gfx_line_f2_count++];
    setRGB0(line, color->r, color->g, color->b);
    SetSemiTrans(line, semitrans & 0xff);
    setXY2(line, rect->x + 0x80, rect->y, rect->w + 0x80, rect->h);
    addPrim(&g_equip_gfx_context->otag[otag_index], line);
}
