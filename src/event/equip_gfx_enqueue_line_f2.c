#include "fft/event_equip.h"
#include "psx/types.h"

/* `rect` carries the two rect as (x0, y0, x1, y1). */
void equip_gfx_enqueue_line_f2(const RECT* rect, const u8* color, s32 semitrans, s32 otag_index) {
    LINE_F2* line;

    line = &g_equip_gfx_context->lines[g_equip_gfx_line_f2_count++];
    line->r0 = color[0];
    line->g0 = color[1];
    line->b0 = color[2];
    SetSemiTrans(line, semitrans & 0xff);
    line->x0 = rect->x + 0x80;
    line->y0 = rect->y;
    line->x1 = rect->w + 0x80;
    line->y1 = rect->h;
    addPrim(&g_equip_gfx_context->otag[otag_index], line);
}
