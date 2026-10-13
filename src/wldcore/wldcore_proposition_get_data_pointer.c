#include "fft/wldcore.h"

/* Resolve record `record` of the proposition data file: a table of offsets
 * relative to the file's start. */
void* wldcore_proposition_get_data_pointer(wldcore_proposition_data_record_e record) {
    return (u8*)g_wldcore_proposition_data_base + g_wldcore_proposition_data_base[record];
}
