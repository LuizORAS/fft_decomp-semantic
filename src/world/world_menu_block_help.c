#include "fft/world.h"

/* WORLD twin of battle_menu_set_modal_open. */
void world_menu_block_help(void) {
    g_world_menu_help_blocked = 1;
}
