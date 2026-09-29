#include "psx/libcd.h"
s32 CdPosToInt(CdlLOC* position) {
    CdlLOC* location = position;
    u32 minute_bcd = location->minute;
    u32 second_bcd = location->second;
    u32 sector_bcd;
    u32 elapsed;
    u32 frames;
    elapsed = ((minute_bcd >> 4) * 10 + (minute_bcd & 15)) * PSYQ_CD_SECONDS_PER_MINUTE;
    elapsed += (second_bcd >> 4) * 10 + (second_bcd & 15);
    frames = elapsed * PSYQ_CD_SECTORS_PER_SECOND;
    sector_bcd = location->sector;
    return frames + ((sector_bcd >> 4) * 10 + (sector_bcd & 15)) - PSYQ_CD_LEAD_IN_SECTORS;
}
