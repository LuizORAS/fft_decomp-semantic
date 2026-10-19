#include "fft/battle_menu.h"

/* Block the help menu while an OPTION, BUNIT, JOBSTTS, EQUIP or DEBUGCHR screen or an action
 * restriction message is up: battle_menu_start_help_menu_thread does not start it and HELPMENU's thread
 * exits at once. battle_menu_clear_modal_open clears it. */
void battle_menu_set_modal_open(void) {
    g_battle_menu_modal_open = 1;
}
