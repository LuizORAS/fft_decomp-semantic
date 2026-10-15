#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_unblock_help. */
void world_menu_unblock_help(void) {
    g_world_menu_help_blocked = 0;
}
