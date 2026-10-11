#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: set the master volume to operand 0 * 0x40 (0xff maps
 * to the maximum 0x3fff) over operand 1 * 3 steps, at least one. */
void wldcore_opcode_change_volume_balance(void) {
    s32 unused[2];
    s32 balance = g_wldcore_active_saved_record.instruction.bytes.operand_1 * 3;
    s32 volume = (g_wldcore_opcode_state.instruction.word & 0xFF00) >> 2;

    if (balance == 0) {
        balance = 1;
    }
    if (volume == 0x3FC0) {
        volume = 0x3FFF;
    }
    main_sound_set_master_volume(volume, balance);
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
