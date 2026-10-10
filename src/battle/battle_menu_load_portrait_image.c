#include "fft/battle.h"
#include "psx/types.h"

/* Load portrait sector 0x164b + 4 * portrait_id straight into VRAM. */
void battle_menu_load_portrait_image(s32 portrait_id) {
    battle_menu_portrait_image_file_t* buffer;

    buffer = battle_menu_alloc_memory(sizeof(*buffer));
    g_battle_thread_call_target = (void (*)(void))main_file_load_checked_to_address;
    battle_thread_call_on_main_stack(portrait_id * 4 + DISC_SECTOR_EVENT_EVTFACE_BIN, 0x2000, buffer);
    LoadImage(&g_battle_menu_portrait_pixel_rect, &buffer->pixels[0][0]);
    LoadImage(&g_battle_menu_portrait_palette_rect, &buffer->palette[0][0]);
    battle_menu_free_memory(buffer);
}
