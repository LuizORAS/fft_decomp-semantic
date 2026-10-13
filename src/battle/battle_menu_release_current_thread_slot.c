#include "fft/battle_menu.h"
#include "fft/battle_thread.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Release the typewriter text slot the current thread owns, if any: clear its
 * owner and its glyph counter. */
void battle_menu_release_current_thread_slot(void) {
    s32 index;
    s32 current_thread_id;
    s32* owner_thread_id;
    s32* slot_state;

    index = 0;
    current_thread_id = g_native_current_thread_id;
    slot_state = g_battle_menu_slot_states;
    owner_thread_id = g_battle_menu_slot_owner_thread_ids;
    for (;;) {
        if (current_thread_id == *owner_thread_id) {
            *owner_thread_id = 0;
            *slot_state = 0;
            return;
        }
        slot_state++;
        index++;
        owner_thread_id++;
        if (index >= 3) {
            return;
        }
    }
}
