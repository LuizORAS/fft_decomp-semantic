#include "fft/world.h"
#include "psx/types.h"

u32* world_shop_load_background_tim(s32 index) {
    return main_file_load_checked_to_heap(
        g_world_shop_background_tim_files[index * 2], g_world_shop_background_tim_files[(index * 2) + 1]);
}
