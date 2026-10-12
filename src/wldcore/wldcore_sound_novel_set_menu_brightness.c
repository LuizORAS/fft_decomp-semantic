#include "fft/wldcore.h"

/* Sound novel opcode 50: set the menus' brightness to the three operands (red, green, blue). */
void wldcore_sound_novel_set_menu_brightness(void) {
    char unused[16];

    world_menu_set_brightness(g_wldcore_sound_novel_record.instruction.bytes.operand_0,
        g_wldcore_sound_novel_record.instruction.bytes.operand_1,
        g_wldcore_sound_novel_record.instruction.bytes.operand_2);
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
