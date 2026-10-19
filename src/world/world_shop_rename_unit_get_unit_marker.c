#include "fft/world.h"
#include "psx/types.h"

/* Unit-grid marker callback of Change name: no unit gets a marker (-1). */
s32 world_shop_rename_unit_get_unit_marker(s32 unused_unit_index) {
    return -1;
}
