#include "fft/world.h"
#include "psx/types.h"

/* Points one of the graphics-data pointers at its resident copy.
 *
 * Each case leaves the bound address in $v0, and
 * world_card_build_save_slot_description reads it for case 1 through a
 * function-pointer cast. A native build must return the pointer (QUIRKS.md). */
u8* world_text_bind_section_data(u32 index) {
    switch (index) {
    case WORLD_TEXT_DATA_SKILLSET_NAMES:
        g_world_text_section_pointers[TEXT_SECTION_SKILLSET_NAMES] = g_world_text_skillset_names_data;
        break;
    case WORLD_TEXT_DATA_JOB_NAMES:
        g_world_text_section_pointers[TEXT_SECTION_JOB_NAMES] = g_world_text_job_names_data;
        break;
    case WORLD_TEXT_DATA_ABILITY_NAMES:
        g_world_text_section_pointers[TEXT_SECTION_ABILITY_NAMES] = g_world_text_ability_names_data;
        break;
    case WORLD_TEXT_DATA_FORMATION_MESSAGES:
        g_world_text_section_pointers[TEXT_SECTION_FORMATION_MESSAGES] = g_world_text_formation_messages_data;
        break;
    case WORLD_TEXT_DATA_DISMISS_UNIT_REPLIES:
        g_world_text_section_pointers[WORLD_TEXT_SECTION_FORMATION_TEXT] = g_world_text_dismiss_unit_replies_data;
        break;
    case WORLD_TEXT_DATA_JOB_DESCRIPTIONS:
        g_world_text_section_pointers[WORLD_TEXT_SECTION_FORMATION_TEXT] = g_world_text_job_descriptions_data;
        break;
    case WORLD_TEXT_DATA_NAME_ENTRY_KEYBOARD:
        g_world_text_section_pointers[TEXT_SECTION_OVERLAY_SCREEN] = g_world_text_name_entry_keyboard_data;
        break;
    case WORLD_TEXT_DATA_SHOP_DIALOGUE:
        g_world_text_section_pointers[WORLD_TEXT_SECTION_SHOP_DIALOGUE] = g_world_text_shop_dialogue_data;
        break;
    }
}
