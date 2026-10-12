#include "fft/battle.h"
#include "fft/battle_event.h"
#include "fft/battle_menu.h"
#include "fft/main.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Uploads eight 32x48 portraits and their palettes from one EVTFACE.BIN block. */
void script_load_portrait_colors_event_instruction(s32 portrait_id) {
    RECT rect;
    menu_portrait_image_file_t* buffer;
    u32(*pixels)[48][4];
    s32 i;
    palette_t* palette;

    buffer = battle_menu_alloc_memory(sizeof(*buffer));
    do {
        battle_thread_yield();
        g_battle_thread_call_target = (void (*)(void))main_file_call_build_header;
    } while (battle_thread_call_on_main_stack(portrait_id * 4 + DISC_SECTOR_EVENT_EVTFACE_BIN, sizeof(*buffer), buffer)
        != 0);
    do {
        battle_thread_yield();
        g_battle_thread_call_target = main_file_is_still_loading;
    } while (battle_thread_call_on_main_stack() != 0);

    palette = buffer->palettes;
    battle_copy_bytes(&rect, &g_battle_menu_portrait_pixel_rect, 8);
    i = 0;
    pixels = buffer->pixels;
    rect.w = 8;
    rect.h = 0x30;
    do {
        LoadImage(&rect, pixels[0][0]);
        rect.x += 8;
        pixels += 1;
        i += 1;
    } while (i < 8);
    LoadImage(&g_battle_menu_portrait_palette_rect, (u32*)palette);
    battle_menu_free_memory(buffer);
    battle_thread_yield();
}
