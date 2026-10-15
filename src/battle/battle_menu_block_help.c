#include "fft/battle.h"

/* Block the help menu while an OPTION, BUNIT, JOBSTTS, EQUIP or DEBUGCHR screen or an action
 * restriction message is up: battle_menu_start_help_menu_thread does not start it and HELPMENU's thread
 * exits at once. battle_menu_unblock_help clears it. */
void battle_menu_block_help(void) {
    g_battle_menu_help_blocked = 1;
}
