#include "fft/battle.h"
#include "psx/types.h"

/* Wait until threads 13, 12, 11 and 10 have stopped, in that order. The menus call it
 * before opening BUNIT, DEBUGCHR or companion overlays 9 and 10. */
void battle_thread_wait_for_10_to_13(void) {
    battle_thread_wait_until_inactive(0xD);
    battle_thread_wait_until_inactive(0xC);
    battle_thread_wait_until_inactive(0xB);
    battle_thread_wait_until_inactive(0xA);
}
