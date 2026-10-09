#include "psx/libgs.h"
#include "psx/libgte.h"
#include "psx/types.h"

void psyq_gs_init_gte(void) {
    InitGeom();
    SetFarColor(0, 0, 0);
    SetGeomOffset(0, 0);
    g_psyq_gs_offset_y = 0;
    g_psyq_gs_offset_x = 0;
}
