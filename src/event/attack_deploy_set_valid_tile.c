#include "fft/event_attack.h"
#include "psx/types.h"

/* Mark deployment tile (column, row) valid or not (g_attack_deploy_valid_tiles). Nothing references it (QUIRKS.md). */
void attack_deploy_set_valid_tile(s32 column, s32 row, u8 is_valid) {
    g_attack_deploy_valid_tiles[row][column] = is_valid;
}
