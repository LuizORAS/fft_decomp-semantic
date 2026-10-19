#include "fft/wldcore.h"

/* Sound novel opcode 50: set the menus' brightness to the three operands (red, green, blue). */
void wldcore_sound_novel_set_menu_brightness(void) {
    world_menu_set_brightness((u8)(g_wldcore_sound_novel_record.instruction.word >> 8),
        (u8)(g_wldcore_sound_novel_record.instruction.word >> 16),
        (u8)(g_wldcore_sound_novel_record.instruction.word >> 24));
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
