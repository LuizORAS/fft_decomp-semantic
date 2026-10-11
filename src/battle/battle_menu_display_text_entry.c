#include "fft/battle.h"

/* Render one BATTLE menu-text entry through the main-thread callback. */
void battle_menu_display_text_entry(s32 text_id, void* image, glyph_blit_t* origin) {
    g_battle_thread_call_target = (void (*)(void))battle_menu_display_text;
    battle_thread_call_on_main_stack(text_id, image, origin, 0);
}
