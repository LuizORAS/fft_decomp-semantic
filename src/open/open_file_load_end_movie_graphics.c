#include "fft/open.h"
#include "psx/types.h"

/* Load the ending's graphics from OPNTEX.BIN and OPNBK.BIN: the TIM at sector
 * 0x65 (the script's glyph page, uploaded at once), OPNBK image 6 (uploaded as
 * the 512x240 picture at VRAM (512, 256)) and the 16 OPNTEX frames from sector
 * 0x76, which stay at g_open_gfx_opntex_data for the script to upload. */
void open_file_load_end_movie_graphics(void) {
    main_file_load_descriptor_t* header = &g_main_file_cd_state;
    RECT frame;

    open_file_wait_then_build_header(header, 0x150c2, 0x11, g_open_file_destination);
    open_file_wait_for_pending();
    open_gfx_load_opntex_into_frame_buffer(g_open_file_destination);

    open_file_build_openbk_header(6, g_open_gfx_opntex_data);
    open_file_wait_for_pending();

    frame.x = 0x200;
    frame.y = 0x100;
    frame.w = 0x200;
    frame.h = 0xf0;
    LoadImage(&frame, g_open_gfx_opntex_data);
    DrawSync(0);

    open_file_wait_then_build_header(header, 0x150d3, 0x170, g_open_gfx_opntex_data);
    open_file_wait_for_pending();
}
