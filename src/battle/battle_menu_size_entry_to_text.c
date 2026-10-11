#include "fft/battle.h"

/*
 * Size a message window to its text entry.
 *
 * Measures the text, optionally widens a single-line entry to eight pixels
 * and reports its text padding, adds the 0x18 border and the low two width
 * bits, then centres the window on x = 0x102.
 */
void battle_menu_size_entry_to_text(world_menu_entry_t* window, s16* width, s16* height, s32* out_pad, s32 pad_short) {
    u8* text;

    text = battle_text_find_entry(window->text_id);
    *out_pad = 0;
    battle_text_measure_pixels(width, height, text);
    if (pad_short != 0 && *width < 8 && *height == 1) {
        *out_pad = (8 - *width) * 5;
        *width = 8;
    }
    *width += 0x18;
    *width += *width & 3;
    *height = *height * 16 + 16;
    window->window_x = 0x102 - (*width >> 1);
    window->window_width = window->inner_width = window->text_image.width = *width;
    window->window_height = window->inner_height = window->text_image.height = *height;
}
