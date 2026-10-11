#include "psx/libgs.h"
#include "psx/types.h"

/* Sets the base of the packet work area the sorters allocate their packets
 * from. */
void GsSetWorkBase(void* base) {
    GsOUT_PACKET_P = base;
}
