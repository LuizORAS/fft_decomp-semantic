/* SCUS_942.21 0x80021024..0x80021083. */
#include "psx/libcd.h"

/* Put the position after the last completed frame in `position` and return its frame number, or -1 when
 * the stream skips sector positions. */
s32 StGetBackloc(CdlLOC* position) {
    s32 frame_count;
    if (g_psyq_cd_stream_skip_sector_position == 0) {
        CdIntToPos(CdPosToInt(&g_psyq_cd_backloc_position) + 1, position);
        frame_count = g_psyq_cd_backloc_frame;
    } else {
        frame_count = -1;
    }
    return frame_count;
}
