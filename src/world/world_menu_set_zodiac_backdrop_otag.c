#include "fft/world.h"
#include "psx/types.h"

/* Set the ordering table world_menu_zodiac_backdrop_thread draws into; the card screen and
 * WLDCORE's world frame set it each frame. */
void world_menu_set_zodiac_backdrop_otag(void* ordering_table) {
    g_world_menu_zodiac_backdrop_otag = ordering_table;
}
