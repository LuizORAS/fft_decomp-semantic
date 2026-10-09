#include "fft/wldcore.h"
#include "psx/types.h"

/* Expand one 23-byte proposition row into halfword fields, then remap the
 * two fields at +0x08/+0x0a through the halfword table of record 1. */
void wldcore_unpack_proposition_row(wldcore_proposition_fields_t* out, s32 index) {
    u8* rows;
    u16* field_lookup;
    s32 i;

    rows = (u8*)wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_ROWS);
    for (i = 0; i < 23; i++) {
        out->values[i] = rows[index * 23 + i];
    }
    field_lookup = (u16*)wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_FEES);
    out->values[4] = field_lookup[out->values[4]];
    out->values[5] = field_lookup[out->values[5]];
}
