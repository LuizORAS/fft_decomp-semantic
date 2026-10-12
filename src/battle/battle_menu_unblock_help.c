#include "fft/battle.h"

/* Clears the "an options/menu overlay is on screen" flag. Called from the
 * BATTLE.BIN overlay loaders (EQUIP.OUT, DEBUGCHR.OUT, ...) once the overlay
 * has been torn down. */
void battle_menu_unblock_help(void) {
    g_battle_menu_help_blocked = 0;
}
