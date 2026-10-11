#include "fft/world.h"
#include "psx/types.h"

/* Load shop background image index into a heap block: nine images, three each for the
 * outfitter, the fur shop and the soldier office; g_world_shop_background_tim_files holds each one's
 * sector and size. */
u32* world_shop_load_background_tim(s32 index) {
    return main_file_alloc_and_load_checked(
        g_world_shop_background_tim_files[index * 2], g_world_shop_background_tim_files[(index * 2) + 1]);
}
