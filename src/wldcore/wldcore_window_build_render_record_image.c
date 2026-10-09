#include "fft/wldcore.h"

/* Prepare a render record and build its CPU-side window image.
 *
 * The caller adds optional text, uploads the image, and activates the record.
 *
 * bounds arrives by value in a1/a2: the record's bounds are copied from the
 * argument save slots, and the transform call receives the same struct.
 */
void wldcore_window_build_render_record_image(
    s32 index, wldcore_window_render_bounds16_t bounds, s32 coordinate_mode, u32* image) {
    wldcore_point32_t* clut;
    u8* color;
    s32 transformed[2];

    g_wldcore_window_render_records[index].flags
        &= ~(WLDCORE_DISPLAY_OBJECT_8BIT_CLUT | WLDCORE_DISPLAY_OBJECT_BLANK | WLDCORE_DISPLAY_OBJECT_HIDDEN);
    g_wldcore_window_render_records[index].base.x = (s16)bounds.position.x - 0x80;
    g_wldcore_window_render_records[index].base.y = (s16)bounds.position.y - 0x78;
    g_wldcore_window_render_records[index].coordinate_mode = coordinate_mode;
    /* Struct view of clut_x/clut_y: as plain s32 stores they could alias the
     * stacked image argument, whose load the target schedules after them. */
    clut = &g_wldcore_window_render_records[index].clut;
    clut->y = 0x1E1;
    clut->x = 0;
    g_wldcore_window_render_records[index].bounds = bounds;
    wldcore_window_transform_point_for_mode(coordinate_mode, bounds, transformed);
    g_wldcore_window_render_records[index].tpage = GetTPage(0, 0, transformed[0], transformed[1]) & 0xFFFF;
    /* One byte pointer for red/green/blue; three field stores change the bytes. */
    color = &g_wldcore_window_render_records[index].color.r;
    color[0] = 0x80;
    color[1] = 0x80;
    color[2] = 0x80;
    g_wldcore_window_render_records[index].anim_counter = 0;
    world_menu_build_window_frame_image((s16)bounds.dimensions.x, (s16)bounds.dimensions.y, (u16*)image);
}
