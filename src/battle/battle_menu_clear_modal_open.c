#include "fft/battle_menu.h"

/* Let the help menu open again (battle_menu_set_modal_open). */
void battle_menu_clear_modal_open(void) {
    g_battle_menu_modal_open = 0;
}
