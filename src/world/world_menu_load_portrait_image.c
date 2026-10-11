#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Upload one EVTFACE.BIN block of eight portraits and their palettes. */
void world_menu_load_portrait_image(s32 index) {
    menu_portrait_image_file_t* buffer;

    buffer = world_menu_alloc_ui_buffer(sizeof(*buffer));
    g_world_thread_call_target = (void (*)(void))main_file_load_checked_to_address;
    world_thread_call_on_main_stack(index * 4 + DISC_SECTOR_EVENT_EVTFACE_BIN, sizeof(*buffer), buffer);
    LoadImage(&g_world_menu_portrait_pixel_rect, buffer->pixels[0][0]);
    LoadImage(&g_world_menu_portrait_palette_rect, (u32*)buffer->palettes);
    world_menu_free_memory(buffer);
}
