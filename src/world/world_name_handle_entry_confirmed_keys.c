#include "fft/thread.h"
#include "fft/world.h"
#include "psx/pad.h"

/* Name entry state 0, the keyboard: show prompt 0xC015, move the cursor, and after the first ten frames
 * apply the key Circle presses: a special key of row 0 (backspace, delete, space, cursor left or right; the
 * sixth confirms), else the row's character, with 0xFE typed as a space. Cross or Start goes to the
 * confirmation (state 1). L1, R1 and Square are handled by the screen's frame step. */
void world_name_handle_entry_confirmed_keys(void) {
    u8 previous;
    s32 character;

    if (g_world_name_entry_confirm_message_shown == 0) {
        g_world_name_entry_input_delay = 0;
        thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x21, 0xC015, 0);
        g_world_name_entry_confirm_message_shown = 1;
    }
    if (g_world_name_entry_input_delay < 10) {
        g_world_name_entry_input_delay++;
    }
    world_name_update_entry_cursor();
    g_world_name_entry_confirm_message_shown = 0;
    if ((g_world_input_primary_repeat & (PSX_PAD_CROSS | PSX_PAD_START)) && g_world_name_entry_input_delay >= 10) {
        previous = g_world_name_entry_state;
        g_world_name_entry_state = 1;
        g_world_name_entry_saved_state = previous;
    } else {
        g_world_name_entry_confirm_message_shown = 1;
    }
    if ((g_world_input_primary_repeat & PSX_PAD_CIRCLE) && g_world_name_entry_input_delay >= 10) {
        g_world_menu_sound_effect_id = MAIN_SFX_SELECT;
        if (g_world_name_entry_list_cursor == 0) {
            if (g_world_name_entry_cursor_index == 5) {
                previous = g_world_name_entry_state;
                g_world_name_entry_state = 1;
                g_world_name_entry_confirm_message_shown = 0;
                g_world_name_entry_saved_state = previous;
            } else {
                world_name_apply_entry_key(g_world_name_entry_key_codes[g_world_name_entry_cursor_index]);
            }
            g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
            return;
        }
        character = world_text_get_encoded_char_at_position(g_world_name_entry_character_rows,
            g_world_name_entry_key_ids[g_world_name_entry_list_cursor], g_world_name_entry_cursor_index);
        if (character == TEXT_END_WAIT_FOR_CONFIRM) {
            character = TEXT_SPACE;
        }
        world_name_apply_entry_key(character);
    }
}
