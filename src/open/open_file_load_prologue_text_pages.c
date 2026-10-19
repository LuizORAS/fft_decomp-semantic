#include "fft/open.h"

/* Upload the two TIMs at sectors 0x43 and 0x54 of OPNTEX.BIN: the text pages at
 * VRAM (256, 256) and (320, 256) that hold Alazlam's prologue, its lines drawn as images.
 * The only caller is open_prologue_start. */
void open_file_load_prologue_text_pages(void) {
    main_file_load_descriptor_t* header = &g_main_file_cd_state;

    open_file_wait_then_build_header(header, 0x150a0, 0x11, g_open_file_destination);
    open_file_wait_for_pending();
    open_gfx_load_opntex_into_frame_buffer(g_open_file_destination);

    open_file_wait_then_build_header(header, 0x150b1, 0x11, g_open_file_destination);
    open_file_wait_for_pending();
    open_gfx_load_opntex_into_frame_buffer(g_open_file_destination);
}
