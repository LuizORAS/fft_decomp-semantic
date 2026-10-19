#include "fft/thread.h"

/* Wait until threads 13, 12, 11 and 10 have stopped, in that order. The menus call it
 * before opening BUNIT, DEBUGCHR or companion overlays 9 and 10. */
void thread_wait_for_unit_view_panels(void) {
    thread_wait_until_inactive(THREAD_SLOT_HOVERED_UNIT_STATS);
    thread_wait_until_inactive(THREAD_SLOT_RIGHT_UNIT_STATS);
    thread_wait_until_inactive(THREAD_SLOT_UNIT_SUMMARY_PANEL);
    thread_wait_until_inactive(THREAD_SLOT_TILE_INFO_PANEL);
}
