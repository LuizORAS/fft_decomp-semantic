#include "fft/wldcore.h"

/* Resolve record `record` of the proposition data file: a table of offsets
 * relative to the file's start. */
void* wldcore_proposition_get_data_pointer(wldcore_proposition_data_record_e record) {
    s32* table = g_wldcore_proposition_data_base;

    return (u8*)table + table[record];
}
