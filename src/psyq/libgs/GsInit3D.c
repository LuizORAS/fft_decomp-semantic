#include "psx/libgs.h"
#include "psx/types.h"

void GsInit3D(void) {
    POSITION.offx = HWD0 / 2;
    POSITION.offy = VWD0 / 2;
    GsSetDrawBuffOffset();
    GsCLIP3near = 0xA;
    GsLIGHT_MODE = 0;
    GsCLIP3far = ZRESOLUTION;
}
