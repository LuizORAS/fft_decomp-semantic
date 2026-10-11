#include "fft/world.h"
#include "psx/types.h"

/* Wait until threads 13, 12, 11 and 10 have stopped, in that order. The menus call it
 * before opening BUNIT, DEBUGCHR or companion overlays 9 and 10. */
void world_thread_wait_for_unit_view_panels(void) {
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);
}
