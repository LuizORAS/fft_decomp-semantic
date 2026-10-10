#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Dispatch every command of a command stream through the handler table until
 * the end opcode, after resetting the ordering-table index, its lock and the
 * semi-transparency state; `flags` is the input the handlers see. Then
 * advances the transition frame, up to 10. */
void jobstts_cmd_run_stream(u8* data, s32 flags) {
    g_jobstts_gfx_otag_index_locked = 0;
    g_jobstts_gfx_otag_index = 0;
    g_jobstts_gfx_semitransparency = 0;
    g_jobstts_cmd_stream_input = flags;
    while (*data != JOBSTTS_CMD_END) {
        data = g_jobstts_cmd_handlers[*data](data);
    }
    if (g_jobstts_gfx_transition_frame < 10) {
        g_jobstts_gfx_transition_frame = g_jobstts_gfx_transition_frame + 1;
    }
}
