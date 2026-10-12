#include "fft/world.h"

/* Show `message_id` in the text thread (dialog type 0x1B) with the formation unit's roster slot as
 * speaker, unless the thread is busy. Nothing calls it. */
void world_formation_show_unit_message(s16 unit_index, s32 message_id) {
    world_text_start_character_thread_if_idle(
        NATIVE_SCREEN_THREAD_SLOT_TEXT, 0x1B, message_id, g_world_formation_unit_pointers[unit_index]->roster_slot);
}
