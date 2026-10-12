#include "fft/battle_menu.h"
#include "psx/types.h"

/* Fill one row of a window image from the eight 16-bit patterns at tiles[index].
 *
 * Entries 0-1 are the left border, written to the first two cells, and entries 6-7
 * the right border, written to row[count - 4] and row[count - 3]. Entries 2-5
 * repeat every fourth cell across the interior. The last two cells of the row are
 * the caller's. BATTLE twin of world_menu_build_window_image_row. */
void battle_menu_build_window_image_row(const u16* tiles, u16* row, s32 count, s32 index) {
    u16* cell;
    u16 tile;
    s32 i;

    row[0] = tiles[index];
    row[1] = tiles[index + 1];
    row[count - 4] = tiles[index + 6];
    row[count - 3] = tiles[index + 7];

    tile = tiles[index + 2];
    cell = &row[2];
    for (i = 0; i < count - 6; i += 4) {
        *cell = tile;
        cell += 4;
    }

    tile = tiles[index + 3];
    cell = &row[3];
    for (i = 0; i < count - 7; i += 4) {
        *cell = tile;
        cell += 4;
    }

    tile = tiles[index + 4];
    cell = &row[4];
    for (i = 0; i < count - 8; i += 4) {
        *cell = tile;
        cell += 4;
    }

    tile = tiles[index + 5];
    cell = &row[5];
    for (i = 0; i < count - 9; i += 4) {
        *cell = tile;
        cell += 4;
    }
}
