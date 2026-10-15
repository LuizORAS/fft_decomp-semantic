#include "fft/event_equip.h"
#include "psx/types.h"

/* Update a wrapping horizontal menu selection, queue `sound_id` when it
 * moves and return the new selection. */
s32 equip_menu_update_horizontal_selection_and_mark_change(
    s32 entry_count, s32 selection_index, s32 input_mask, s8 sound_id) {
    s32 index = selection_index & 0xFF;
    s16 previous_selection = g_equip_menu_selection_values[index];
    s16 selection;

    selection = equip_menu_update_wrapped_horizontal_selection(entry_count & 0xFFFF, index, input_mask);
    if (previous_selection != selection) {
        g_equip_sound_queued_effect_id = sound_id;
    }
    return selection;
}
