#include "fft/world.h"
#include "psx/types.h"

/* Release the message box slot (one of three) the current thread holds, if any: clear its
 * owner and glyph counter. world_text_message_box_thread calls it as it ends. */
void world_text_release_message_box_slot(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (g_world_thread_current_id == g_world_text_message_box_slot_threads[i]) {
            g_world_text_message_box_slot_threads[i] = 0;
            g_world_text_message_box_slot_glyph_counters[i] = 0;
            return;
        }
    }
}
