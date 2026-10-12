#include "fft/wldcore.h"

/* The visit's first screen: load the current location's picture (the marker's sub_kind), capture
 * the map as the backdrop (wldcore_gfx_capture_map_backdrop), then reset the GPU and clear both frame
 * buffers to black, so the visit fades in from black over the backdrop.
 * The matched range includes the jr delay slot at 0x800683f8. */
void wldcore_init_screen(void) {
    s32 reserved_stack_words[2];
    s32 count;
    SetDispMask(0);
    g_wldcore_map_projection_state.marker.sub_kind
        = wldcore_gfx_load_location_picture(g_wldcore_map_projection_state.marker.kind);
    wldcore_wait_for_file_load();
    count = GsGetActiveBuff();
    g_active_graphics_buffer_index = count;
    GsSetWorkBase(&g_wldcore_gfx_world_primitive_buffers + count * 0xE000);
    wldcore_gfx_capture_map_backdrop();
    VSync(0);
    ResetGraph(1);
    GsClearOt(0, 0, &g_wldcore_gfx_aux_ordering_tables[g_active_graphics_buffer_index]);
    GsClearOt(0, 0, &g_wldcore_gfx_ordering_tables[g_active_graphics_buffer_index]);
    wldcore_gfx_clear_vram_and_scratch(0);
    GsSwapDispBuff();
    SetDispMask(0);
}
