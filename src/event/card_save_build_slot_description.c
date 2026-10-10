#include "fft/event_card.h"
#include "psx/types.h"

void card_save_build_slot_description(s32 mode, u8* destination) {
    s32 index;
    u8 stack_padding[4];
    const u8* source;

    if (mode == 1) {
        source = card_text_find_entry_by_index(g_card_text_data, 14, 1);
        while ((*destination++ = *source++) != 0xfe) { }
    } else if (mode == 0) {
        index = 0;
        while (g_card_save_buffer_pointer->summary.name[index] != 0xfe) {
            *destination++ = g_card_save_buffer_pointer->summary.name[index];
            index++;
        }

        *destination++ = 0xe7;
        *destination++ = 0x58;
        source = card_text_find_entry_by_index(g_card_text_job_name, g_card_save_buffer_pointer->summary.job_id, 1);
        while (*source != 0xfe) {
            *destination++ = *source++;
        }

        *destination++ = 0xe7;
        *destination++ = 0xac;
        source = card_text_find_entry_by_index(g_card_text_data, 13, 1);
        while (*source != 0xfe) {
            *destination++ = *source++;
        }

        *destination++ = g_card_save_buffer_pointer->summary.level / 10;
        *destination++ = g_card_save_buffer_pointer->summary.level % 10;
        *destination++ = 0xfe;
        source = card_text_find_entry_by_index(g_card_text_data, g_card_save_buffer_pointer->summary.month, 1);
        while (*source != 0xfe) {
            *destination++ = *source++;
        }

        *destination++ = 0xfa;
        *destination++ = g_card_save_buffer_pointer->summary.day / 10;
        *destination++ = g_card_save_buffer_pointer->summary.day % 10;
        *destination++ = 0xe7;
        *destination++ = 0x58;
        source
            = card_text_find_entry_by_index(g_card_text_location_name, g_card_save_buffer_pointer->summary.location, 1);
        while ((*destination++ = *source++) != 0xfe) { }
    }
    *destination = 0xfe;
}
