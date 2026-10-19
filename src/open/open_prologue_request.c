#include "fft/open.h"
#include "psx/types.h"

/* Request the new-game prologue over entry_table: entries 0 to target - 1 run,
 * with their text in ordering-table entries ot_layer and ot_layer + 1. */
void open_prologue_request(s32 target, s32 ot_layer, open_prologue_entry_t** entry_table) {
    g_open_prologue.flags = OPEN_PROLOGUE_NEXT_ENTRY;
    g_open_prologue.current_index = 0;
    g_open_prologue.target_index = target;
    g_open_prologue.entry_table = entry_table;
    g_open_prologue.ot_layer = ot_layer;
    g_open_prologue.intensity = 0x80;
}
