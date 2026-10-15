#include "psx/libgpu.h"
#include "psx/types.h"

/* Upload an OPNTEX image, preceded by its CLUT when flag bit 3 is set.
 *
 * Word 1 holds the flags, and each tim is a block header followed by the
 * pixel data. The CLUT rectangle is flattened to one row of w * h entries.
 * Reading x and w through word locals keeps the target's full-word loads. The
 * product is pinned to $a3, where the allocator would not otherwise place
 * it. Type debt (QUIRKS.md): callers pass the OPNTEX bytes (u8*), read here
 * as words. */
void open_gfx_load_opntex_into_frame_buffer(const void* source_data) {
    u32* tim = (u32*)source_data;
    RECT rect;
    s32 offset;
    /* Unused; the target's frame is 16 bytes larger than rect needs. */
    s32 stack_padding[4];

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
