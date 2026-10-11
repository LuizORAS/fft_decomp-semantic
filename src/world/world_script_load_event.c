#include "fft/world.h"

/* WORLD twin of battle_script_load_event: read one event block from the archive on
 * the main thread, wait for the load, then patch the text-section pointer. */
void world_script_load_event(s32 id) {
    event_file_block_t* block;
    u32 off;

    do {
        world_thread_yield();
        g_world_thread_call_target = (void (*)(void))main_file_call_build_header;
    } while (world_thread_call_on_main_stack(
                 id * EVENT_BLOCK_SECTORS + DISC_SECTOR_EVENT_TEST_EVT, EVENT_BLOCK_BYTES, g_main_script_event_block)
        != 0);

    do {
        world_thread_yield();
        g_world_thread_call_target = main_file_is_still_loading;
    } while (world_thread_call_on_main_stack() != 0);

    block = (event_file_block_t*)g_world_event_script;
    off = block->text_offset_or_marker;
    if (off != EVENT_TEXT_OFFSET_PROCESSED) {
        /* Byte-pointer addition reverses the target's addu operands. */
        g_world_text_section_pointers[TEXT_SECTION_EVENT_MESSAGES] = (u8*)(off + (u32)block);
        block->text_offset_or_marker = EVENT_TEXT_OFFSET_PROCESSED;
    }
}
