#include "psx/libgs.h"

/* Prepare LIBGS's 3D state: put the screen centre (half the screen size) in the draw offsets, apply them
 * (GsSetDrawBuffOffset), set light mode 0 and two library globals (GsCLIP3near to 10, GsCLIP3far to 0x3fff).
 * wldcore_gfx_init_subsystem calls it. */
void GsInit3D(void) {
    POSITION.offx = HWD0 / 2;
    POSITION.offy = VWD0 / 2;
    GsSetDrawBuffOffset();
    GsCLIP3near = 0xA;
    GsLIGHT_MODE = 0;
    GsCLIP3far = ZRESOLUTION;
}
