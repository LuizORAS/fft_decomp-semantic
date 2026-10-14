#include "fft/wldcore.h"
#include "psx/libgs.h"

/* Sound novel opcode 10: load picture operand 0 of WLDPIC.BIN (wldcore_gfx_load_picture_set), place
 * its render record at (operand 2 - 0x80, operand 1 - 0x78), upload the TIM's pixels and CLUT, and bind
 * the render record to the image. */
void wldcore_sound_novel_load_picture(void) {
    GsIMAGE image;
    wldcore_window_render_bounds16_t bounds;
    wldcore_point32_t clut_position;
    wldcore_point32_t unused_slot; /* reserves the target frame's 0x50 slot */
    u32 instruction;
    s32 picture;
    s32 x;
    s32 y;

    instruction = g_wldcore_sound_novel_state.instruction.word;
    x = instruction >> 24;
    y = (instruction >> 16) & 0xFF;
    picture = (instruction & 0xFF00) >> 8;
    g_wldcore_sound_novel_state.picture = picture;
    g_wldcore_sound_novel_state.x = x;
    g_wldcore_sound_novel_state.y = y;
    wldcore_gfx_load_picture_set(picture);
    wldcore_file_wait_for_load();
    g_wldcore_window_render_records[g_wldcore_sound_novel_state.render_index].base.x = x - 0x80;
    g_wldcore_window_render_records[g_wldcore_sound_novel_state.render_index].base.y = y - 0x78;
    GsGetTimInfo(g_wldcore_picture_buffer + 1, &image);
    bounds.position.x = image.px;
    bounds.position.y = image.py;
    bounds.dimensions.x = image.pw;
    bounds.dimensions.y = image.ph;
    clut_position.x = image.cx;
    clut_position.y = image.cy;
    LoadTPage(image.pixel, 0, 0, bounds.position.x, bounds.position.y, bounds.dimensions.x * 4, bounds.dimensions.y);
    LoadClut(image.clut, clut_position.x, clut_position.y);
    wldcore_window_init_vram_render_record(
        g_wldcore_sound_novel_state.render_index, bounds, clut_position, WLDCORE_DISPLAY_OBJECT_8BIT_CLUT);
    g_wldcore_window_render_records[g_wldcore_sound_novel_state.render_index].anim_counter = 0;
    g_wldcore_sound_novel_state.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
