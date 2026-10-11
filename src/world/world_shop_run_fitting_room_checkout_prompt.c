#include "fft/world.h"

/* Run one frame of the fitting room's checkout; the menus' cursors are reset on entry. An empty
 * cart is bought at once (the kept items go to the inventory). When the party can afford the cost, it
 * asks "It's N gil. OK?" (Yes / No): Yes pays, makes the tried-on items the units' own, saves the
 * records to the roster and returns WORLD_SHOP_CHECKOUT_DONE; No asks "Nothing more to try on?".
 * Short of gil, it asks "You're short of gil. Wanna put one back?". Both questions take the Quit / No
 * menu (g_world_shop_checkout_quit_menu): No returns WORLD_SHOP_CHECKOUT_CANCELLED (back to the
 * fitting room); Quit, or X, undoes the fitting (world_shop_undo_fitting_room) and returns DONE.
 * Returns WORLD_SHOP_CHECKOUT_PROMPT_OPEN while a menu is open. */
s32 world_shop_run_fitting_room_checkout_prompt(void) {
    s32 balance;
    s32 cost;

    if (g_world_shop_checkout_prompt_initialized == 0) {
        if (world_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_MESSAGE) != 0) {
            return WORLD_SHOP_CHECKOUT_PROMPT_OPEN;
        }
        g_world_shop_checkout_confirm_menu.selected_index = 0;
        g_world_shop_checkout_quit_menu.selected_index = 0;
        world_thread_start(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
        balance = world_shop_add_gil(0) - (cost = world_shop_add_fitting_room_cost(0));
        if (cost == 0) {
            world_shop_buy_from_fitting_room();
            return WORLD_SHOP_CHECKOUT_DONE;
        }
        if (balance >= 0) {
            g_world_menu_sound_muted = 1;
            g_world_shop_checkout_prompt_menu = &g_world_shop_checkout_confirm_menu;
            g_world_text_substitution_values[0] = cost;
            world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xf819, 0);
            g_world_shop_checkout_insufficient_gil = 0;
        } else {
            g_world_shop_checkout_prompt_menu = &g_world_shop_checkout_quit_menu;
            world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xf81a, 0);
            g_world_shop_checkout_insufficient_gil = 1;
        }
        g_world_shop_checkout_prompt_initialized = 1;
        g_world_shop_checkout_prompt_active = 1;
    }
    if (g_world_shop_checkout_prompt_active != 0) {
        g_world_shop_checkout_prompt_active
            = world_menu_run_thread(WORLD_THREAD_SLOT_UPPER_MENU, g_world_shop_checkout_prompt_menu);
        if (g_world_shop_checkout_prompt_active != 0) {
            return WORLD_SHOP_CHECKOUT_PROMPT_OPEN;
        }
        g_world_shop_checkout_prompt_result = g_world_menu_selection_results[WORLD_MENU_SELECTION_PROMPT_RESULT];
        if (g_world_shop_checkout_insufficient_gil == 0) {
            g_world_menu_sound_muted = 0;
            if (g_world_shop_checkout_prompt_result == 0) {
                balance = -world_shop_add_fitting_room_cost(0);
                world_shop_add_gil(balance);
                world_shop_add_fitting_room_cost(balance);
                world_shop_buy_from_fitting_room();
                world_formation_save_records_to_party_data();
                g_world_menu_sound_effect_id = MAIN_SFX_SHOP_TRANSACTION;
                world_item_reconcile_sorted_list(0);
                world_item_reconcile_sorted_list(1);
                world_item_reconcile_sorted_list(2);
                world_item_reconcile_sorted_list(3);
                world_item_reconcile_sorted_list(4);
                g_world_shop_checkout_prompt_initialized = 0;
                return WORLD_SHOP_CHECKOUT_DONE;
            }
            if (g_world_shop_checkout_prompt_result == 1) {
                g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
            }
            world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, 0xf81c, 0);
            return WORLD_SHOP_CHECKOUT_PROMPT_OPEN;
        }
        if (g_world_shop_checkout_prompt_result != 1) {
            world_shop_undo_fitting_room();
            g_world_shop_checkout_prompt_initialized = 0;
            return WORLD_SHOP_CHECKOUT_DONE;
        }
        world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, -1, 0);
        g_world_shop_checkout_prompt_initialized = 0;
        return WORLD_SHOP_CHECKOUT_CANCELLED;
    }
    if (world_menu_run_thread(WORLD_THREAD_SLOT_UPPER_MENU, &g_world_shop_checkout_quit_menu) != 0) {
        return WORLD_SHOP_CHECKOUT_PROMPT_OPEN;
    }
    g_world_shop_checkout_prompt_initialized = 0;
    if (g_world_menu_selection_results[WORLD_MENU_SELECTION_PROMPT_RESULT] != 1) {
        if (g_world_shop_checkout_prompt_result != 0) {
            world_shop_undo_fitting_room();
            return WORLD_SHOP_CHECKOUT_DONE;
        }
        return WORLD_SHOP_CHECKOUT_DONE;
    }
    world_thread_set_parameters(NATIVE_SCREEN_THREAD_SLOT_MESSAGE, 0x19, -1, 0);
    return WORLD_SHOP_CHECKOUT_CANCELLED;
}
