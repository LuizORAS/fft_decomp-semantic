#include "fft/wldcore.h"
#include "psx/types.h"

/* Load the picture of location record `index` (stored as set + 1) and return the stored value; 0
 * means the location has no picture. */
s32 wldcore_gfx_load_location_picture(s32 index) {
    s32 value = g_wldcore_location_records[index].picture;

    if (value != 0)
        wldcore_gfx_load_picture_set(value - 1);
    return value;
}
