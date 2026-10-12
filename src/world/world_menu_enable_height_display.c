#include "fft/world.h"
#include "psx/types.h"

/* Paired with world_menu_suppress_height_display (0x800e8a34).  While the flag is non-zero the world
   display thread at 0x800e8ab0 skips its four primitive submissions. */
void world_menu_enable_height_display(void) {
    g_world_menu_height_display_suppressed = 0;
}
