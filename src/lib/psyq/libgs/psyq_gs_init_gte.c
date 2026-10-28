#include "psx/libgs.h"
#include "psx/libgte.h"

/* The geometry half of GsInitGraph: start the GTE (InitGeom), with a black far color, a zero screen
 * offset and zero sorter offsets. */
void psyq_gs_init_gte(void) {
    InitGeom();
    SetFarColor(0, 0, 0);
    SetGeomOffset(0, 0);
    GsORGOFSY = 0;
    GsORGOFSX = 0;
}
