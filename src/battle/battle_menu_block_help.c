#include "fft/battle.h"

/* Sets the "an options/menu overlay is on screen" flag; the matching clear is
 * battle_menu_unblock_help. */
void battle_menu_block_help(void) {
    g_battle_menu_help_blocked = 1;
}
