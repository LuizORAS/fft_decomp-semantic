/* SCUS_942.21 0x80021084..0x8002110b. */
#include "psx/libcd.h"

void StSetStream(s32 mode, s32 start_frame, s32 end_frame, void* frame_callback, void* end_callback) {
    StSetMask(1, start_frame, end_frame);
    g_psyq_cd_stream_emulation_base = 0;
    g_psyq_cd_stream_frame_callback = frame_callback;
    g_psyq_cd_stream_mode = mode & 1;
    g_psyq_cd_stream_current_channel = 0;
    g_psyq_cd_stream_requested_channel = 0;
    g_psyq_cd_stream_expected_sector_index = 0;
    g_psyq_cd_stream_frame_count = 0;
    g_psyq_cd_stream_end_callback = end_callback;
}
