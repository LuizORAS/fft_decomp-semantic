#include "fft/wldcore.h"
#include "psx/types.h"

/* Load the picture data set of location record `index` (stored as set + 1)
 * and return the stored value; 0 means the record has no picture. */
s32 wldcore_proposition_load_picture(s32 index) {
    s32 value = g_wldcore_location_records[index].picture;

    if (value != 0)
        wldcore_proposition_load_picture_data_set(value - 1);
    return value;
}
