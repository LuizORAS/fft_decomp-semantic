#include "fft/main.h"

/* Start loading the ENTD file that holds g_main_entd_current_event_id into a new
 * 0x14000-byte heap block (g_main_entd_set). The four files cover 128 events
 * each.
 *
 * Returns 1 once the read is queued, and also for event 0, which loads nothing.
 * Returns 0 with the block freed when the loader is busy; the caller retries. */
int main_entd_open_file(void) {
    int lba;
    entd_file_t* entd_set;

    if (g_main_entd_current_event_id != 0) {
        if (g_main_entd_current_event_id < ENTD2_FIRST_EVENT) {
            entd_set = main_heap_alloc(sizeof(entd_file_t));
            g_main_entd_set = entd_set;
            lba = DISC_SECTOR_BATTLE_ENTD1_ENT;
        } else if (g_main_entd_current_event_id < ENTD3_FIRST_EVENT) {
            entd_set = main_heap_alloc(sizeof(entd_file_t));
            g_main_entd_set = entd_set;
            lba = DISC_SECTOR_BATTLE_ENTD2_ENT;
        } else if (g_main_entd_current_event_id < ENTD4_FIRST_EVENT) {
            entd_set = main_heap_alloc(sizeof(entd_file_t));
            g_main_entd_set = entd_set;
            lba = DISC_SECTOR_BATTLE_ENTD3_ENT;
        } else {
            entd_set = main_heap_alloc(sizeof(entd_file_t));
            g_main_entd_set = entd_set;
            lba = DISC_SECTOR_BATTLE_ENTD4_ENT;
        }

        if (main_file_call_build_header(lba, sizeof(entd_file_t), entd_set) != 0) {
            main_heap_call_free(g_main_entd_set);
            return 0;
        }
    }
    return 1;
}
