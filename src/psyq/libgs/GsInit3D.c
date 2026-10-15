#include "psx/libgs.h"
#include "psx/types.h"

/* Prepare LIBGS's 3D state: put the screen centre (half the screen size) in the draw offsets, apply them
 * (GsSetDrawBuffOffset), set light mode 0 and two library words (D_801CD854 to 10, D_801CD7DC to 0x3fff).
 * wldcore_gfx_init_subsystem calls it. */
void GsInit3D(void) {
    POSITION.offx = HWD0 / 2;
    POSITION.offy = VWD0 / 2;
    GsSetDrawBuffOffset();
    D_801CD854 = 0xA;
    GsLIGHT_MODE = 0;
    D_801CD7DC = 0x3FFF;
}
