#include "fft/world.h"
#include "psx/libc.h"

/* Copy formation unit `index` into the preview record (g_world_formation_temp_unit). Both callers
 * also pass WORLD_FORMATION_PREVIEW_RECORD, which it does not read. */
void world_formation_copy_unit_to_temp(s16 index, s32 unused_slot) {
    bcopy(g_world_formation_unit_pointers[index], g_world_formation_temp_unit, sizeof(world_formation_unit_t));
}
