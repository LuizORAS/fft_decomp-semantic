#include "fft/wldcore.h"

/* Reset the bookmark of the novel being opened (g_main_sound_novel_records[g_wldcore_sound_novel_index])
 * to the start of a reading. */
void wldcore_sound_novel_reset_selected_record(s32 unused) {
    wldcore_sound_novel_reset_record(&g_main_sound_novel_records[g_wldcore_sound_novel_index]);
}
