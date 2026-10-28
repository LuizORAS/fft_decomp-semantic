#include "fft/event_equip.h"
#include "fft/gfx.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Takes the next LINE_F2 packet from the pool (its SetLineF2 code is kept), draws it in `color` from
 * endpoints[0] to endpoints[1] with an x bias of 0x80, semi-transparent when `semitrans` is set, and
 * links it into ordering-table entry otag_index (libgpu addPrim). WORLD's copy is world_gfx_enqueue_line_f2. */
void gfx_enqueue_line_f2(const point16_t endpoints[2], const rgb24_t* color, u8 semitrans, s32 otag_index) {
    LINE_F2* line;

    line = &g_menu_gfx_context->lines[g_equip_gfx_line_f2_count++];
    setRGB0(line, color->r, color->g, color->b);
    SetSemiTrans(line, semitrans);
    setXY2(line, endpoints[0].x + 0x80, endpoints[0].y, endpoints[1].x + 0x80, endpoints[1].y);
    addPrim(&g_menu_gfx_context->otag[otag_index], line);
}
