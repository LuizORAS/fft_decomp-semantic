#include "fft/world.h"
#include "psx/types.h"

/* Load shop background image `index` to the heap; the table holds a sector
 * and a size per image. */
u32* world_shop_load_background_tim(s32 index) {
    return main_file_load_checked_to_heap(
        g_world_shop_background_tim_files[index * 2], g_world_shop_background_tim_files[(index * 2) + 1]);
}
