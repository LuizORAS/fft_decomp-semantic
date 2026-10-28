#include "fft/world.h"

/* WORLD twin of battle_menu_clear_modal_open. */
void world_menu_unblock_help(void) {
    g_world_menu_help_blocked = 0;
}
