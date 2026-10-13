#include "fft/battle_gfx.h"
#include "psx/types.h"

/* Clear the 15-by-16 allocation grid consumed by the tpage-7 VRAM allocator. */
void battle_gfx_clear_tpage7_vram_allocation_grid(void) {
    s32 row;
    s32 column;

    for (row = 0; row < 15; row++) {
        for (column = 15; column >= 0; column--) {
            g_battle_gfx_tpage7_vram_allocation_grid[row][column] = 0;
        }
    }
}
