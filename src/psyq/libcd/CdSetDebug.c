#include "psx/libcd.h"

/* Set the library's debug level and return the old one: above 0 it prints errors, above 1 it traces every
 * command. main_file_init_cd calls it. */
s32 CdSetDebug(s32 level) {
    s32 previous = g_psyq_cd_debug_level;
    g_psyq_cd_debug_level = level;
    return previous;
}
