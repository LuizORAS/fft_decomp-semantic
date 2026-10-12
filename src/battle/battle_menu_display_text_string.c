#include "fft/battle.h"
#include "fft/battle_menu.h"
#include "psx/types.h"

/* Render text into a menu image at origin through the main-stack text
 * renderer. */
void battle_menu_display_text_string(u8* image, glyph_blit_t* origin, u8* text) {
    g_battle_thread_call_target = (void (*)(void))battle_menu_display_text;
    battle_thread_call_on_main_stack(0, image, origin, text);
}
