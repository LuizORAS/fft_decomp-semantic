#include "fft/world.h"
#include "psx/types.h"

void world_menu_display_text_entry_mode_1(s32 text_id, void* image, world_glyph_blit_t* origin) {
    g_world_thread_call_target = (void (*)(void))world_menu_display_text;
    world_thread_call_on_main_stack(text_id, image, origin, 1);
}
