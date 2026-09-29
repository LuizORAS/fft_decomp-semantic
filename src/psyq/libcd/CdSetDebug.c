#include "psx/libcd.h"

s32 CdSetDebug(s32 level) {
    s32 previous = g_psyq_cd_debug_level;
    g_psyq_cd_debug_level = level;
    return previous;
}
