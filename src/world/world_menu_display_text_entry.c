#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_display_text_entry. */
void world_menu_display_text_entry(s32 text_id, void* image, world_glyph_blit_t* origin) {
    g_world_thread_call_target = (void (*)(void))world_menu_display_text;
    world_thread_call_on_main_stack(text_id, image, origin, 0);
}
