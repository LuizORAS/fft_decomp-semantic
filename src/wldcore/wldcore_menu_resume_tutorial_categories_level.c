#include "fft/thread.h"
#include "fft/wldcore.h"

/* Resume handler of the tutorial menu: its windows and text go back to CLUT 0, the cursor to its idle
 * sequence, and WORLD thread 14 shows Daravon's greeting (text 0xb8dd) again.
 *
 * Each store addresses its record field by symbol, the ASPSX $at indexed form the target uses.
 * `side_window` is read before the stores because the target schedules that load into the first store's
 * address computation; `slot` is reloaded per store because the stores invalidate the cached parameter
 * loads. `pad` covers the eight frame bytes the target reserves below the saved return address. */
void wldcore_menu_resume_tutorial_categories_level(wldcore_menu_stack_record_t* level) {
    s32 main_window;
    s32 side_window;
    s32 slot;
    /* The original frame reserves eight unused bytes. */
    s32 pad[2];

    main_window = level->list_window.main_window;
    side_window = level->list_window.side_window;
    g_wldcore_window_render_records[level->list_window.frame_render].palette = 0;
    g_wldcore_window_records[side_window].palette = 0;
    g_wldcore_window_records[main_window].palette = 0;
    slot = level->list_window.main_window;
    g_wldcore_window_records[slot].sequence = WLDCORE_ANIM_SEQUENCE_CURSOR_IDLE;
    slot = level->list_window.main_window;
    g_wldcore_window_records[slot].anim_counter = 0;
    g_wldcore_window_records[slot].frame_index = 0;
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_TUTORIAL_WELCOME, 0);
}
