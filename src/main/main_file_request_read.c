#include "fft/main.h"
#include "psx/libcd.h"

/* Start an asynchronous read of sector_count sectors from lba into destination: fill the
 * descriptor and set it to its first state; main_file_poll_load then advances it once per
 * frame. Returns 1, changing nothing, while the descriptor is busy. loading_display_mode sets
 * the Now Loading frame counter (the message shows from 64; -1 never shows it), except 1,
 * which keeps the current count; 0 also turns the display off, clears both 256x240 display
 * buffers and resets their environments. A null destination only seeks. */
s32 main_file_request_read(
    main_file_load_descriptor_t* descriptor, s32 lba, s32 sector_count, void* destination, s32 loading_display_mode) {
    RECT rect;

    if (descriptor->state != MAIN_FILE_LOAD_STATE_IDLE) {
        return 1;
    }
    if (loading_display_mode != 1) {
        g_main_gfx_loading_display_frame_counter = loading_display_mode;
    }
    if (loading_display_mode == 0) {
        SetDispMask(0);
        main_gfx_build_now_loading(1, 0, 0);
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x100;
        rect.h = 0x1e0;
        ClearImage(&rect, 0, 0, 0);
        DrawSync(0);
        SetDefDrawEnv(&g_main_gfx_draw_envs[0], 0, 0, 0x100, 0xf0);
        SetDefDispEnv(&g_main_gfx_display_envs[0], 0, 0xf0, 0x100, 0xf0);
        SetDefDrawEnv(&g_main_gfx_draw_envs[1], 0, 0xf0, 0x100, 0xf0);
        SetDefDispEnv(&g_main_gfx_display_envs[1], 0, 0, 0x100, 0xf0);
        g_main_gfx_draw_envs[1].ofs[1] = 0xf0;
        g_main_gfx_draw_envs[1].ofs[0] = 0;
        g_main_gfx_draw_envs[0].ofs[1] = 0;
        g_main_gfx_draw_envs[0].ofs[0] = 0;
        g_main_gfx_draw_envs[1].dfe = 1;
        g_main_gfx_draw_envs[1].isbg = 1;
        g_main_gfx_draw_envs[0].dfe = 1;
        g_main_gfx_draw_envs[0].isbg = 1;
        PutDrawEnv(&g_main_gfx_draw_envs[1]);
        PutDispEnv(&g_main_gfx_display_envs[1]);
    }
    descriptor->_unknown_00 = 0;
    descriptor->sector_index = 0;
    descriptor->error_count = 0;
    descriptor->state = MAIN_FILE_LOAD_STATE_SET_DOUBLE_SPEED;
    descriptor->lba = lba;
    CdIntToPos(lba, descriptor->position);
    descriptor->sector_count = sector_count;
    descriptor->destination = destination;
    return 0;
}
