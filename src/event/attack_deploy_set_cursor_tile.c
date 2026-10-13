#include "fft/event_attack.h"
#include "psx/types.h"

/* Put the first deployment cursor on tile (column, row). Nothing references it (QUIRKS.md). */
void attack_deploy_set_cursor_tile(s32 column, s32 row) {
    g_attack_deploy_cursor_column[0] = column;
    g_attack_deploy_cursor_row[0] = row;
}
