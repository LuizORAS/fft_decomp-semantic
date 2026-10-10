#include "fft/world.h"
#include "psx/types.h"

/* world_menu_display_text_entry with a text argument of 1: the renderer draws on past a
 * TEXT_CHOICE_MARK instead of ending there. The sound novel history draws its entries this way. */
void world_menu_display_text_entry_past_choice_marks(s32 text_id, void* image, world_glyph_blit_t* origin) {
    g_world_thread_call_target = (void (*)(void))world_menu_display_text;
    world_thread_call_on_main_stack(text_id, image, origin, 1);
}
