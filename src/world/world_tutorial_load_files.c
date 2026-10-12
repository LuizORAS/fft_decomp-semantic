#include "fft/world.h"

/* Load tutorial `index`'s message file and script file (1-based; sector and size pairs in
 * g_world_tutorial_message_files and g_world_tutorial_script_files) into the record's two pointers. */
void world_tutorial_load_files(s32 index, world_gfx_tim_pair_record_t* record) {
    s32 slot = index - 1;

    record->tim_pointers[0] = main_file_alloc_and_load_checked(
        g_world_tutorial_message_files[slot * 2], g_world_tutorial_message_files[(slot * 2) + 1]);
    record->tim_pointers[1] = main_file_alloc_and_load_checked(
        g_world_tutorial_script_files[slot * 2], g_world_tutorial_script_files[(slot * 2) + 1]);
}
