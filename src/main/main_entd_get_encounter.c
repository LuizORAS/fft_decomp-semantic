#include "fft/main.h"

/* Returns -1 for no current encounter and NULL while its file is loading.
 * Event ids 0-127 index the loaded ENTD file directly; each further
 * range of 128 ids (the last one open-ended) indexes it from its own base. */
entd_encounter_t* main_entd_get_encounter(void) {
    int* event_id = &g_main_entd_current_event_id;
    int id;

    if (*event_id == 0) {
        return (entd_encounter_t*)-1;
    }
    if (main_file_is_still_loading() == 0) {
        id = *event_id;
        if (id < ENTD2_FIRST_EVENT) {
            return &g_main_entd_set->encounters[id];
        }
        return &g_main_entd_set->encounters[id < ENTD3_FIRST_EVENT ? id - ENTD2_FIRST_EVENT
                : id < ENTD4_FIRST_EVENT                           ? id - ENTD3_FIRST_EVENT
                                                                   : id - ENTD4_FIRST_EVENT];
    }
    return 0;
}
