#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/*
 * Open WLDFACE.BIN and move it to the frame buffer.
 *
 * The file at disc sector 0x18ba is fetched on the main thread and moved to
 * VRAM as its four sheets, each with its portraits' CLUTs below them.
 */
void world_gfx_load_wldface_to_frame_buffer(void) {
    main_gfx_wldface_sheet_t* sheets;

    g_world_thread_call_target = main_file_alloc_and_load_checked;
    sheets = (main_gfx_wldface_sheet_t*)world_thread_call_on_main_stack(
        DISC_SECTOR_EVENT_WLDFACE_BIN, MAIN_GFX_WLDFACE_FILE_SIZE);
    LoadImage(&g_world_wldface_vram_rects, sheets[0].pixels);
    LoadImage(&g_world_wldface_vram_rect_1, sheets[1].pixels);
    LoadImage(&g_world_wldface_vram_rect_2, sheets[2].pixels);
    LoadImage(&g_world_wldface_vram_rect_3, sheets[3].pixels);
    DrawSync(0);
    main_heap_call_free(sheets);
}
