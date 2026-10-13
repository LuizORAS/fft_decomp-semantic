#include "fft/event_require.h"

/* Thread of the menu record that the recruit prompt's third row opens: row action 3 of the second record
 * of g_require_join_menu_descriptor lands on g_require_party_removal_menus[1], the two arrays being one menu
 * table. Run require_overlay_open_jobstts in thread 6, which shows the unit's jobs and abilities in
 * JOBSTTS.OUT, wait for it and exit. */
void require_overlay_open_jobstts_thread(void) {
    battle_thread_start(6, require_overlay_open_jobstts);
    battle_thread_wait_until_inactive(6);
    battle_thread_exit_current();
}
