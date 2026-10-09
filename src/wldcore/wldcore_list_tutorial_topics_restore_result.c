#include "fft/wldcore.h"
#include "psx/pad.h"

void wldcore_list_tutorial_topics_restore_result(void) {
    g_wldcore_menu_result = PSX_PAD_CIRCLE | PSX_PAD_CROSS | PSX_PAD_SELECT;
}
