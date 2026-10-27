#include "fft/main.h"
#include "psx/libgpu.h"

/* Load BATTLE/ZODIAC.BIN into VRAM at (640, 0), then EVENT/FRAME.BIN
 * (main_gfx_load_frame_bin_into_vram).
 *
 * ZODIAC.BIN is a headerless image, not an executable overlay: its 0x10000 bytes are 128 x
 * 256 16-bit VRAM transfer values, later drawn as 8-bit indexed texture data. */
void main_gfx_load_zodiac_frame(void) {
    RECT destination = { 640, 0, 128, 256 };
    u32* image;

    image = main_file_alloc_and_load_checked(DISC_SECTOR_BATTLE_ZODIAC_BIN, 0x10000);
    LoadImage(&destination, image);
    DrawSync(0);
    main_heap_free(image);
    main_gfx_load_frame_bin_into_vram();
}
