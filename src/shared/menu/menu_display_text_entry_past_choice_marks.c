#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Same as menu_display_text_entry with a text argument of 1: WORLD's renderer then draws on past a
 * TEXT_CHOICE_MARK instead of ending there; BATTLE's skips the mark either way, and nothing in BATTLE
 * calls this. */
void menu_display_text_entry_past_choice_marks(s32 text_id, u8* image, world_glyph_blit_t* origin) {
    g_thread_call_target = (void (*)(void))battle_menu_display_text;
    thread_call_on_main_stack(text_id, image, origin, 1);
}
