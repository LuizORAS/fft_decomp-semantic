#include "fft/world.h"
#include "psx/types.h"

u32* world_gfx_load_tim_by_table_index(s32 index) {
    return main_file_alloc_and_load_checked(
        g_world_gfx_tim_file_table[index * 2], g_world_gfx_tim_file_table[(index * 2) + 1]);
}
