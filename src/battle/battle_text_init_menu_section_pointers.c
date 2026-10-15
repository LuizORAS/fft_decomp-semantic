#include "fft/battle.h"
#include "psx/types.h"

/* Point text sections 0-30 at the menu text BATTLE.BIN carries, a copy of EVENT/SAMPLE.LZW
 * (g_battle_menu_text_section_offsets: 32 offsets counted from the end of the table). Section 31, the AT
 * list, is left alone, and section 0 keeps the running event's text unless the player drives the menus
 * (MENU_INPUT_PLAYER), so a tutorial's messages stay bound. */
void battle_text_init_menu_section_pointers(void) {
    s32* source;
    u8** destination;
    /* These two assignments fill the target load delay before the loop. */
    s32* text;
    s32 menu_input_mode;
    s32 index;
    s32 offset;

    source = g_battle_menu_text_section_offsets;
    index = 0;
    menu_input_mode = g_battle_menu_input_mode;
    text = source + 32;
    destination = g_battle_text_section_pointers;
    do {
        if (menu_input_mode == MENU_INPUT_PLAYER || index != TEXT_SECTION_EVENT_MESSAGES) {
            offset = *source;
            offset += (s32)text;
            *destination = (u8*)offset;
        }
        source++;
        index++;
        destination++;
    } while (index < 31);
}
