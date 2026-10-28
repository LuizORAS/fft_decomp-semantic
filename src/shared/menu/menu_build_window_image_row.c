#include "fft/menu.h"

/* Fill one row of a window image from the eight 16-bit pattern values at tiles[index].
 *
 * Values 0-1 are the left border, written to the first two cells, and values 6-7 the
 * right border, written to row[count - 4] and row[count - 3]. Values 2-5
 * repeat every fourth cell across the interior. The last two cells of the row are
 * the caller's. BATTLE and WORLD compile this text, WORLD twice: world_screen_menu_build_window_image_row,
 * the menu-screen library's copy, is the one world_menu_build_window_frame_image calls. */
void menu_build_window_image_row(const u16* tiles, u16* row, s32 count, s32 index) {
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
