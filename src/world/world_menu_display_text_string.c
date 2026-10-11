#include "fft/world.h"
#include "psx/types.h"

/* Render text into a menu image at origin through the main-stack text
 * renderer. */
void world_menu_display_text_string(u8* image, glyph_blit_t* origin, u8* text) {
    g_world_thread_call_target = (void (*)(void))world_menu_display_text;
    world_thread_call_on_main_stack(0, image, origin, text);
}
