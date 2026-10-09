#include "fft/wldcore.h"
#include "psx/libgpu.h"

/* Upload a render record's window image at the VRAM position its coordinate
 * mode selects. The record's bounds are passed to the transform by value. */
void wldcore_window_load_image_record_to_vram(s32 index, u32* image) {
    s32 transformed[2];
    RECT rect;
    s32 x;
    s32 y;

    wldcore_window_transform_point_for_mode(g_wldcore_window_render_records[index].coordinate_mode,
        g_wldcore_window_render_records[index].bounds, transformed);
    x = transformed[0];
    y = transformed[1];
    rect.x = x;
    rect.y = y;
    rect.w = (s16)g_wldcore_window_render_records[index].bounds.dimensions.x / 4;
    rect.h = g_wldcore_window_render_records[index].bounds.dimensions.y;
    LoadImage(&rect, image);
}
