#include "fft/battle.h"
#include "psx/types.h"

void battle_thread_wait_for_10_to_13(void) {
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_HOVERED_UNIT_STATS);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_UNIT_SUMMARY_PANEL);
    battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_TILE_INFO_PANEL);
}
