#include "fft/battle.h"
#include "psx/types.h"

/* battle_menu_display_text_entry with a text argument of 1 instead of 0. In WORLD that draws on past
 * a TEXT_CHOICE_MARK; BATTLE's renderer skips the mark either way, and nothing calls this. */
void battle_menu_display_text_entry_past_choice_marks(s32 text_id, u8* image, glyph_blit_t* origin) {
    g_battle_thread_call_target = (void (*)(void))battle_menu_display_text;
    battle_thread_call_on_main_stack(text_id, image, origin, 1);
}
