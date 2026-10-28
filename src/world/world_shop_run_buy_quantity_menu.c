#include "fft/thread.h"
#include "fft/world.h"

/* The buy quantity prompt for the highlighted item. On entry it caps the quantity at
 * min(gil / price, 99 - owned), where owned counts equipped copies too, and refuses with
 * WORLD_SHOP_QUANTITY_HOLDING_MAX (0xF807, "You can't hold any more") when 99 are owned or
 * WORLD_SHOP_QUANTITY_CANNOT_AFFORD (0xF806, "short of gil") when not one is affordable. Afterwards it
 * returns the quantity menu thread's state, showing the total price (0xF809) while the confirmation
 * runs. */
s32 world_shop_run_buy_quantity_menu(void) {
    s32 owned;
    s32 count;
    s32 limit;

    if (g_world_shop_quantity_menu_running == 0) {
        g_world_shop_purchase_total_shown = 0;
        g_world_menu_prompt_active = 1;
        g_world_shop_purchase_item = g_world_menu_entry_ids[g_world_menu_cursor_position];
        owned = world_item_count_owned_and_equipped(g_world_shop_purchase_item);
        if (owned >= 99) {
            thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x19, 0xF807, 0);
            return WORLD_SHOP_QUANTITY_HOLDING_MAX;
        }
        count = world_shop_add_gil(0);
        g_world_shop_purchase_unit_price = world_item_get_price(g_world_shop_purchase_item);
        count /= g_world_shop_purchase_unit_price;
        limit = 99 - owned;
        if (limit < count) {
            count = limit;
        }
        if (count == 0) {
            thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x19, 0xF806, 0);
            return WORLD_SHOP_QUANTITY_CANNOT_AFFORD;
        }
        thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x19, 0xF808, 0);
        g_world_shop_quantity_limit = count;
        g_world_shop_purchase_total_shown = 0;
        g_world_sound_suppressed = 1;
    }
    g_world_shop_quantity_menu_running
        = world_menu_run_thread(WORLD_THREAD_SLOT_UPPER_MENU, g_world_shop_quantity_menu_data);
    if (thread_is_running(WORLD_THREAD_SLOT_UPPER_MENU_CHILD) != 0 && g_world_shop_purchase_total_shown == 0) {
        g_text_substitution_values[0]
            = g_world_menu_selection_results[WORLD_MENU_SELECTION_SHOP_QUANTITY] * g_world_shop_purchase_unit_price;
        thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x19, 0xF809, 0);
        g_world_shop_purchase_total_shown = 1;
        g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
    }
    if (thread_is_running(WORLD_THREAD_SLOT_UPPER_MENU_CHILD) == 0 && g_world_shop_purchase_total_shown != 0) {
        g_world_shop_purchase_total_shown = 0;
        thread_set_parameters(THREAD_SCREEN_SLOT_MESSAGE, 0x19, 0xF808, 0);
    }
    if (g_world_shop_quantity_menu_running == 0) {
        if (g_world_menu_selection_results[WORLD_MENU_SELECTION_PROMPT_RESULT] == 0) {
            g_world_menu_sound_effect_id = MAIN_SFX_SHOP_TRANSACTION;
        }
        if (g_world_menu_selection_results[WORLD_MENU_SELECTION_PROMPT_RESULT] == 1) {
            g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
        }
        g_world_sound_suppressed = 0;
    }
    return g_world_shop_quantity_menu_running;
}
