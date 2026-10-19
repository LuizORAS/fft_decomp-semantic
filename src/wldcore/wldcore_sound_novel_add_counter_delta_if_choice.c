#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Scalar counter access preserves the separate load/store symbol bases. */
extern u16 g_wldcore_sound_novel_counter_delta;

/* Sound novel opcode 51: when the dialog option chosen (EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION)
 * equals the operand's third byte, add the signed middle 16-bit value of the 32-bit instruction to the
 * record's counter delta (g_wldcore_sound_novel_counter_delta). */
void wldcore_sound_novel_add_counter_delta_if_choice(void) {
    u32* instruction;
    u8 expected;

    instruction = &g_wldcore_sound_novel_state.instruction.word;

    expected = *instruction >> 24;
    if (script_get_variable(EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION) == expected) {
        g_wldcore_sound_novel_counter_delta += (s32)((*instruction << 8) >> 16) - 0x8000;
    }
    g_wldcore_sound_novel_state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
