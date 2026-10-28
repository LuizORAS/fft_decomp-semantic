#include "fft/battle.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Load portrait portrait_id (four sectors each in EVTFACE.BIN) straight into VRAM. */
void battle_menu_load_portrait_image(s32 portrait_id) {
    menu_portrait_image_file_t* buffer;

    buffer = battle_menu_alloc_memory(sizeof(*buffer));
    g_thread_call_target = (void (*)(void))main_file_load_to_address_checked;
    thread_call_on_main_stack(portrait_id * 4 + DISC_SECTOR_EVENT_EVTFACE_BIN, sizeof(*buffer), buffer);
    LoadImage(&g_battle_menu_portrait_pixel_rect, buffer->pixels[0][0]);
    LoadImage(&g_battle_menu_portrait_palette_rect, (u32*)buffer->palettes);
    battle_menu_free_memory(buffer);
}
