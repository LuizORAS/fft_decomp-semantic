#include "fft/wldcore.h"

/* Keep every button from the WORLD menu frame while the next level runs (the help text or the
 * report question). The caller hands in its list values; the target reads no argument. */
void wldcore_proposition_report_list_block_all_buttons(wldcore_menu_stack_record_t* unused_level) {
    g_wldcore_menu_blocked_buttons = -1;
}
