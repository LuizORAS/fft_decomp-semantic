#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* WORLD's copy of battle_script_load_portrait_colors_event_instruction (PortraitCol): read one
 * four-sector block of EVTFACE.BIN on the main stack, upload its eight pixel columns and its palette to
 * VRAM, and free the buffer. */
void world_script_load_portrait_colors_event_instruction(s32 index) {
    RECT rect;
    u8* buffer;
    u8* source;
    u8* upper_half;
    s32 i;

    buffer = world_menu_alloc_memory(0x2000);
    do {
        world_thread_yield();
        g_world_thread_call_target = (void (*)(void))main_file_request_read_bytes;
    } while (world_thread_call_on_main_stack(index * 4 + DISC_SECTOR_EVENT_EVTFACE_BIN, 0x2000, buffer) != 0);
    do {
        world_thread_yield();
        g_world_thread_call_target = (void (*)(void))main_file_is_still_loading;
    } while (world_thread_call_on_main_stack() != 0);
    upper_half = buffer + 0x1800;
    world_copy_bytes(&rect, &g_world_menu_portrait_pixel_rect, 8);
    i = 0;
    source = buffer;
    rect.w = 8;
    rect.h = 0x30;
    for (i = 0; i < 8; i++) {
        LoadImage(&rect, source);
        rect.x += 8;
        source += 0x300;
    }
    LoadImage(&g_world_menu_portrait_palette_rect, upper_half);
    world_menu_free_memory(buffer);
    world_thread_yield();
}
