#include "fft/main.h"
#include "psx/types.h"

/* Load EVENT/FRAME.BIN and upload its pixels and its two CLUT blocks. */
void main_gfx_load_frame_bin_into_vram(void) {
    main_gfx_frame_bin_t* frame
        = main_file_alloc_and_load_checked(DISC_SECTOR_EVENT_FRAME_BIN, sizeof(main_gfx_frame_bin_t));

    LoadImage(&g_main_gfx_frame_image_rect, frame->pixels);
    LoadImage(&g_main_gfx_frame_palette_rect, frame->cluts);
    LoadImage(&g_main_gfx_frame_palette_tail_rect, frame->cluts_tail);
    DrawSync(0);
    main_heap_call_free(frame);
}
