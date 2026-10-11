/* Script opcode handler: compares `EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION` against the opcode's
 * third byte and, on a match, adds the signed middle 16 bits of the
 * 32-bit instruction to g_wldcore_sound_novel_counter_delta. */
#include "fft/wldcore.h"
#include "psx/types.h"

/* Scalar counter access preserves the separate load/store symbol bases. */
extern u16 g_wldcore_sound_novel_counter_delta;

void wldcore_opcode_add_counter_delta_if_choice(void) {
    u32* instruction;
    u8 expected;

    instruction = &g_wldcore_opcode_state.instruction;

    expected = *instruction >> 24;
    if (world_script_get_variable(EVENT_SCRIPT_VAR_SELECTED_DIALOG_OPTION) == expected) {
        g_wldcore_sound_novel_counter_delta += (s32)((*instruction << 8) >> 16) - 0x8000;
    }
    g_wldcore_state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
