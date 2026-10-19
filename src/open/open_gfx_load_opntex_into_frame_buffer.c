#include "fft/open.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Upload an OPNTEX image, preceded by its CLUT when flag bit 3 is set.
 *
 * The second 32-bit value holds the flags, and each block is a TIM block: its byte length (itself included), the
 * VRAM position packed as y << 16 | x, the size packed as h << 16 | w, then the pixel data. The CLUT
 * rectangle is flattened to one row of w * h entries. Type debt (QUIRKS.md): callers pass the
 * OPNTEX bytes (u8*), read here as 32-bit values. */
void open_gfx_load_opntex_into_frame_buffer(u32* tim) {
    RECT rect;
    s32 offset;
    /* Unused; the target's frame is 16 bytes larger than rect needs. */
    s32 stack_padding[4];

    /* Packed 32-bit RECT reads preserve the retail lw/lhu mix. */
    if (tim[1] & 8) {
        offset = (tim[2] >> 2) + 2;
        rect.x = tim[3] & 0xFFFF;
        rect.y = tim[3] >> 16;
        rect.w = tim[4] & 0xFFFF;
        rect.h = tim[4] >> 16;
        rect.w = rect.w * rect.h;
        rect.h = 1;
        LoadImage(&rect, tim + 5);
    } else {
        offset = 2;
    }
    rect.x = tim[offset + 1] & 0xFFFF;
    rect.y = tim[offset + 1] >> 16;
    rect.w = tim[offset + 2] & 0xFFFF;
    rect.h = tim[offset + 2] >> 16;
    LoadImage(&rect, &tim[offset + 3]);
    DrawSync(0);
}
