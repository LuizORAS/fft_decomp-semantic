#include "psx/libgpu.h"
#include "psx/libgs.h"
#include "psx/types.h"

/* Draw the ordering table `ot` from its tag (DrawOTag). WLDCORE draws each frame this way. */
void GsDrawOt(GsOT* ot) {
    DrawOTag(ot->tag);
}
