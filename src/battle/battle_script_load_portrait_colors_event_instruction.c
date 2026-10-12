#include "fft/battle.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* PortraitCol (EVENT_OPCODE_PORTRAIT_COL): read the four-sector block `portrait_id` of EVTFACE.BIN
 * (0x2000 bytes) into a menu buffer, upload its eight 8x48 pixel columns from
 * g_battle_menu_portrait_pixel_rect (x advancing by 8) and its palette, at +0x1800, to
 * g_battle_menu_portrait_palette_rect, then free the buffer. */
void battle_script_load_portrait_colors_event_instruction(s32 portrait_id) {
    RECT rect;
    u8* buffer;
    u8* pixels;
    s32 i;
    u8* palette;

    buffer = battle_menu_alloc_memory(0x2000);
    do {
        battle_thread_yield();
        g_battle_thread_call_target = (void (*)(void))main_file_request_read_bytes;
    } while (battle_thread_call_on_main_stack(portrait_id * 4 + DISC_SECTOR_EVENT_EVTFACE_BIN, 0x2000, buffer) != 0);
    do {
        battle_thread_yield();
        g_battle_thread_call_target = main_file_is_still_loading;
    } while (battle_thread_call_on_main_stack() != 0);

    palette = buffer + 0x1800;
    battle_copy_bytes(&rect, &g_battle_menu_portrait_pixel_rect, 8);
    i = 0;
    pixels = buffer;
    rect.w = 8;
    rect.h = 0x30;
    do {
        LoadImage(&rect, (u32*)pixels);
        rect.x += 8;
        pixels += 0x300;
        i += 1;
    } while (i < 8);
    LoadImage(&g_battle_menu_portrait_palette_rect, (u32*)palette);
    battle_menu_free_memory(buffer);
    battle_thread_yield();
}
