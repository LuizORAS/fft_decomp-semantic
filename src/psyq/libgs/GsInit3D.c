#include "psx/libgs.h"
#include "psx/types.h"

void GsInit3D(void) {
    POSITION.offx = HWD0 / 2;
    POSITION.offy = VWD0 / 2;
    GsSetDrawBuffOffset();
    D_801CD854 = 0xA;
    GsLIGHT_MODE = 0;
    D_801CD7DC = 0x3FFF;
}
