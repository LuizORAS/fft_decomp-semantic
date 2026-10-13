#include "fft/wldcore.h"
#include "psx/types.h"

/* Resume handler of a proposition's detail: its text render record (the level's second word) goes
 * back to CLUT 0. */
void wldcore_menu_resume_proposition_detail_level(s32* param) {
    g_wldcore_window_render_records[param[1]].palette = 0;
}
