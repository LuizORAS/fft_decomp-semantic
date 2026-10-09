#include "fft/main.h"

/* Loads the scenario's SMD file into the first free scenario music slot (1 or
 * 2) and returns that slot, or 0 when both are taken or the load fails. */
int main_sound_open_scenario_smd_files(int scenario) {
    int i;
    suzuki_smd_header_t* buffer;

    for (i = 0; i < 2; i++) {
        if (g_main_sound_music.slots.smd[i + 1] == 0) {
            buffer = main_heap_alloc_smd(g_main_sound_scenario_smd_files[scenario].size);
            if (buffer != 0) {
                if (main_file_call_build_header(g_main_sound_scenario_smd_files[scenario].sector,
                        g_main_sound_scenario_smd_files[scenario].size, buffer)
                    == 0) {
                    g_main_sound_music.slots.smd[i + 1] = buffer;
                    g_main_sound_music.slots.handles.loading_slot = i + 1;
                } else {
                    main_system_handle_error(MAIN_SYSTEM_ERROR_SMD_LOAD);
                    return 0;
                }
            } else {
                main_system_handle_error(MAIN_SYSTEM_ERROR_SMD_LOAD);
                return 0;
            }
            return i + 1;
        }
    }
    return 0;
}
