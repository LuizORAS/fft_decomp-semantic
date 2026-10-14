#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Free the window slot (of three) owned by the current thread, if any. */
void world_menu_release_current_thread_slot(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (g_thread_current_id == g_text_slot_owner_thread_ids[i]) {
            g_text_slot_owner_thread_ids[i] = 0;
            g_text_slot_glyph_counts[i] = 0;
            return;
        }
    }
}
