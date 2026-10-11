#include "fft/world.h"
#include "psx/types.h"

/* Fur shop: the quantity prompt of a sale, capped by the inventory count. */
s32 world_shop_run_fur_sell_quantity_menu(void) {
    s32 count;

    if (g_world_shop_fur_quantity_menu_running == 0) {
        g_world_shop_sell_total_shown = 0;
        g_world_shop_sell_quantity_item = g_world_menu_entry_ids[g_world_menu_cursor_position];
        g_world_menu_prompt_active = 1;
        count = world_item_adjust_inventory_count(g_world_shop_sell_quantity_item, 0);
        if (count == 0) {
            world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xF842, 0);
            return -1;
        }
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xF843, 0);
        g_world_shop_quantity_limit = count;
        g_world_shop_sell_total_shown = 0;
        g_world_menu_sound_muted = 1;
    }
    g_world_shop_fur_quantity_menu_running
        = world_menu_run_thread(WORLD_THREAD_SLOT_UPPER_MENU, &g_world_shop_quantity_menu_data);
    if (world_thread_is_running(WORLD_THREAD_SLOT_UPPER_MENU_CHILD) != 0 && g_world_shop_sell_total_shown == 0) {
        g_world_text_substitution_values[0] = world_item_get_half_price(g_world_shop_sell_quantity_item)
            * g_world_menu_selection_results[WORLD_MENU_SELECTION_SHOP_QUANTITY];
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xF844, 0);
        g_world_shop_sell_total_shown = 1;
        g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
    }
    if (world_thread_is_running(WORLD_THREAD_SLOT_UPPER_MENU_CHILD) == 0 && g_world_shop_sell_total_shown != 0) {
        g_world_shop_sell_total_shown = 0;
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xF843, 0);
    }
    if (g_world_shop_fur_quantity_menu_running == 0) {
        if (g_world_menu_selection_results[WORLD_MENU_SELECTION_PROMPT_RESULT] == 0) {
            g_world_menu_sound_effect_id = MAIN_SFX_SHOP_TRANSACTION;
        }
        if (g_world_menu_selection_results[WORLD_MENU_SELECTION_PROMPT_RESULT] == 1) {
            g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
        }
        g_world_menu_sound_muted = 0;
    }
    return g_world_shop_fur_quantity_menu_running;
}
