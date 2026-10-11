#include "fft/event_require.h"
#include "psx/types.h"

/*
 * Load bonus image `image_id` (a 0x6800-byte file, 13 sectors apart from
 * sector 0x16c0) and upload it to VRAM.
 *
 * Waits until 0x8000 bytes can be reserved at the high overlay address, reads
 * the file there on the main stack, uploads the pixels and the palette that
 * follows them, and frees the buffer after one more frame.
 */
void require_reward_load_bonus_image(s32 image_id) {
    void* allocation;
    s32 (*build_operation)();

    do {
        battle_thread_yield();
        allocation = main_heap_reserve_at(g_main_heap_high_overlay_load_address, 0x8000);
    } while (allocation != g_main_heap_high_overlay_load_address);

    build_operation = main_file_request_read_bytes;
    do {
        battle_thread_yield();
        g_battle_thread_call_target = (void (*)(void))build_operation;
    } while (battle_thread_call_on_main_stack(image_id * 13 + DISC_SECTOR_EVENT_BONUS_BIN, 0x6800, allocation) != 0);

    {
        s32 (*poll_operation)() = main_file_is_still_loading;
        do {
            battle_thread_yield();
            g_battle_thread_call_target = (void (*)(void))poll_operation;
        } while (battle_thread_call_on_main_stack() != 0);
    }

    /* The 256-row upload includes the reserved suffix beyond the 0x6800-byte file. */
    LoadImage(&g_require_reward_bonus_image_rect, allocation);
    LoadImage(&g_require_reward_bonus_palette_rect, (u32*)((require_reward_bonus_image_file_t*)allocation)->palette);
    battle_thread_yield();
    main_heap_free(allocation);
}
