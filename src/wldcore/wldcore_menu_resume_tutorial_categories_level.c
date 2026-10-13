#include "fft/wldcore.h"

/* Resume handler of the tutorial menu: its windows and text go back to CLUT 0, the cursor to its idle
 * sequence, and WORLD thread 14 shows Daravon's greeting (text 0xb8dd) again.
 *
 * Each store addresses its record field by symbol, the ASPSX $at indexed form the target uses.
 * `second` is read before the stores because the target schedules that load into the first store's
 * address computation; `slot` is reloaded per store because the stores invalidate the cached parameter
 * loads. `pad` covers the eight frame bytes the target reserves below the saved return address. */
void wldcore_menu_resume_tutorial_categories_level(s32* param) {
    s32 index;
    s32 second;
    s32 slot;
    s32 pad[2];

    index = param[0];
    second = param[1];
    g_wldcore_window_render_records[param[2]].palette = 0;
    g_wldcore_window_records[second].palette = 0;
    g_wldcore_window_records[index].palette = 0;
    slot = param[0];
    g_wldcore_window_records[slot].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    slot = param[0];
    g_wldcore_window_records[slot].anim_counter = 0;
    g_wldcore_window_records[slot].frame_index = 0;
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_TUTORIAL_WELCOME, 0);
}
