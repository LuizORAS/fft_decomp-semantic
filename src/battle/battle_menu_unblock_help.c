#include "fft/battle.h"

/* Let the help menu open again (battle_menu_block_help). */
void battle_menu_unblock_help(void) {
    g_battle_menu_help_blocked = 0;
}
