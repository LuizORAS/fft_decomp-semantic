#include "fft/world.h"

/* Load the 1-based `index`th TIM from each of the two tables into the record. */
void world_script_load_tutorial_files(s32 index, world_gfx_tim_pair_record_t* record) {
    s32 slot = index - 1;

    record->tim_pointers[0] = main_file_alloc_and_load_checked(
        g_world_script_tutorial_message_files[slot * 2], g_world_script_tutorial_message_files[(slot * 2) + 1]);
    record->tim_pointers[1] = main_file_alloc_and_load_checked(
        g_world_script_tutorial_script_files[slot * 2], g_world_script_tutorial_script_files[(slot * 2) + 1]);
}
