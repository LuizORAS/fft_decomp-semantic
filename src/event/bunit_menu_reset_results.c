#include "fft/event_bunit.h"

/* Set the unit list menu's results to -1 (nothing chosen): the menu row (g_bunit_menu_primary_result)
 * and the sort-order list's (g_bunit_menu_reorder_result). */
void bunit_menu_reset_results(void) {
    g_bunit_menu_primary_result = -1;
    g_bunit_menu_reorder_result = -1;
}
