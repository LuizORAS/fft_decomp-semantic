#include "fft/wldcore.h"
#include "psx/types.h"

/* Bring up the GPU for the world map: GsInitGraph2 at 256 x 240 with two display buffers, 3D
 * with no geometry offset, the screen cleared with the map's clip rectangle
 * (wldcore_gfx_clear_screen_and_set_map_clip_rect), and the double-buffered ordering tables: the main
 * one with 16 levels and the auxiliary one, which holds the map, with 4. */
void wldcore_gfx_init_subsystem(void) {
    SetDispMask(0);
    GsInitGraph2(256, 240, 4, 0, 0);
    GsDefDispBuff(0, 0, 0, 240);
    GsInit3D();
    SetGeomOffset(0, 0);
    wldcore_gfx_clear_screen_and_set_map_clip_rect(0);

    g_wldcore_gfx_aux_ordering_tables[1].log2n = 2;
    g_wldcore_gfx_aux_ordering_tables[0].log2n = 2;
    g_wldcore_gfx_aux_ordering_tables[0].org = g_wldcore_gfx_aux_ot_tags[0];
    g_wldcore_gfx_aux_ordering_tables[1].org = g_wldcore_gfx_aux_ot_tags[1];

    g_wldcore_gfx_ordering_tables[1].log2n = 4;
    g_wldcore_gfx_ordering_tables[0].log2n = 4;
    g_wldcore_gfx_ordering_tables[0].org = g_wldcore_gfx_ot_tags[0];
    g_wldcore_gfx_ordering_tables[1].org = g_wldcore_gfx_ot_tags[1];
}
