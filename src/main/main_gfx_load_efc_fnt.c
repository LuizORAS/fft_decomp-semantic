#include "fft/main.h"

/* Set up the debug text font: FntLoad at x 0x280 with its CLUT moved to line 0x7f by patching
 * FntLoad's code (g_fntload_clut_y_offset_immediate, the low immediate byte of its CLUT-Y addiu; QUIRKS.md),
 * then EFC_FNT.TIM's image and CLUT over it, read again until the load succeeds, and a FntPrint stream at
 * (0x88, 0x10), 256 by 256, as the default one. */
void main_gfx_load_efc_fnt(void) {
    TIM_IMAGE image;
    void* data;
    int loaded = 0;

    g_fntload_clut_y_offset_immediate = 0x7f;
    FntLoad(0x280, 0);

    while (!loaded) {
        data = main_file_alloc_and_load_checked(DISC_SECTOR_BATTLE_EFC_FNT_TIM, 0x8800);
        if (data != 0) {
            if (OpenTIM(data) == 0 && ReadTIM(&image) != 0) {
                LoadTPage(image.paddr, 0, 0, 0x280, 0, image.prect->w * 4, image.prect->h);
                loaded = 1;
                LoadClut2(image.caddr, 0x280, 0x7f);
            }
            main_heap_call_free(data);
        } else {
            VSync(0);
            main_file_poll_load(&g_main_file_cd_state);
        }
    }

    SetDumpFnt(FntOpen(0x88, 0x10, 0x100, 0x100, 0, 0x200));
    DrawSync(0);
    DrawSync(0);
}
