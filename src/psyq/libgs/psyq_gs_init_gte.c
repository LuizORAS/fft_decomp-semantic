#include "psx/libgs.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* The geometry half of GsInitGraph: start the GTE (InitGeom), with a black far colour, a zero screen
 * offset and zero sorter offsets. */
void psyq_gs_init_gte(void) {
    InitGeom();
    SetFarColor(0, 0, 0);
    SetGeomOffset(0, 0);
    g_psyq_gs_offset_y = 0;
    g_psyq_gs_offset_x = 0;
}
