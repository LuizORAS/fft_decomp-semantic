#include "fft/wldcore.h"

/* Count the read flags among the first 0x1000 bits of g_main_sound_novel_section_bits and return
 * them as a percentage of 76. Nothing calls it, and it counts past the array (see QUIRKS.md). */
s32 wldcore_sound_novel_calculate_completion_percent(void) {
    s32 count = 0;
    s32 i = 0;

    do {
        if (wldcore_test_bit(g_main_sound_novel_section_bits, i) != 0) {
            count++;
        }
        i++;
    } while (i < 0x1000);
    return count * 100 / 76;
}
