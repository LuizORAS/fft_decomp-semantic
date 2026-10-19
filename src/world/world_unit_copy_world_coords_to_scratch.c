#include "fft/battle_unit.h"
#include "fft/world.h"

/* Copy integer world coordinates into WORLD's unsigned 16-bit triple.
 * The -1 test is retained although the getter returns null for a missing unit.
 */
vec3u16_t* world_unit_copy_world_coords_to_scratch(u32 runtime_id) {
    SVECTOR* source;

    source = battle_unit_get_world_coords_by_runtime_id(runtime_id);

    if (source != (SVECTOR*)-1) {
        g_world_unit_runtime_coords.x = source->vx;
        g_world_unit_runtime_coords.y = source->vy;
        g_world_unit_runtime_coords.z = source->vz;
        return &g_world_unit_runtime_coords;
    }
    return (vec3u16_t*)-1;
}
