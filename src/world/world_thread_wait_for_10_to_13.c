#include "fft/world.h"
#include "psx/types.h"

void world_thread_wait_for_10_to_13(void) {
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);
}
