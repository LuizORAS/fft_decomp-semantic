#include "fft/battle_thread.h"
#include "fft/thread.h"

/* Wait until the unit view's four panel threads (slots 13 down to 10) have stopped. */
void battle_thread_wait_for_unit_view_panels(void) {
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);
}
