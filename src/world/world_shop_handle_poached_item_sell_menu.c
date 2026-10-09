#include "fft/world.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Shop item-list menu step; sibling of world_shop_handle_item_sell_menu.
 *
 * The callback block is written in ascending address order (794 -> 7ac), as
 * the sibling writes it. That makes gcc materialise the two twice-used
 * addresses (count_2 in a0, count in v1) up front at 0xb0-0xbc and defer
 * their stores; the target's emission order then follows. First-use order,
 * not store order, decides this.
 */
void world_shop_handle_poached_item_sell_menu(void) {
    s32 message;

    if (g_world_shop_fur_sell_list_initialized == 0) {
        message = world_menu_has_items(0) != 0 ? 0xf840 : 0xf845;

        world_thread_set_parameters(2, 0x19, message, 0);
        g_world_shop_item_category = world_menu_set_cursor_position_2(0, 0);
        world_item_build_category_list(-1, (s16)(g_world_shop_item_category + 1),
            (s8)(g_world_shop_item_category != 4 ? g_world_shop_item_category + 1 : 7),
            (world_item_list_entry_t*)g_world_menu_entry_ids, 0);
        world_menu_init_scrollable_list_core(
            g_world_menu_entry_ids, 0, g_world_text_section_pointers[WORLD_TEXT_SECTION_ITEM_NAMES]);
        g_world_menu_script_callbacks[0] = world_menu_get_entry_item_equipped_count;
        g_world_menu_script_callbacks[1] = world_menu_get_entry_item_count;
        g_world_menu_script_callbacks[2] = (s32 (*)(s32))world_item_build_type_icon_source;
        g_world_menu_script_callbacks[3] = (s32 (*)(s32))world_menu_get_entry_item_icon_source;
        g_world_menu_script_callbacks[4] = world_item_get_selected_half_price;
        g_world_menu_script_callbacks[5] = world_menu_get_entry_item_equipped_count;
        g_world_menu_script_callbacks[6] = world_menu_get_entry_item_count;
        g_world_shop_fur_sell_list_initialized = 1;
    }

    world_script_set_vsync_mode_and_event_speed(2);
    if ((g_world_input_primary_repeat & PSX_PAD_CROSS) != 0) {
        g_world_shop_fur_sell_list_initialized = 0;
        world_thread_set_parameters(2, 0x19, 0xf830, 0);
        g_world_shop_menu_step = 14;
        g_world_menu_sound_effect_id = MAIN_SFX_CANCEL;
    } else if ((g_world_input_primary_repeat & PSX_PAD_CIRCLE) != 0) {
        if (g_world_menu_option_count == 0) {
            g_world_menu_sound_effect_id = MAIN_SFX_INVALID;
        } else {
            g_world_shop_menu_step = 19;
            g_world_shop_quantity_menu_data.selection = 0;
            g_world_menu_sound_effect_id = MAIN_SFX_CONFIRM;
        }
    }
    world_menu_switch_item_category(g_world_input_newly_pressed);
    world_menu_run_script_with_palette_mode(
        g_world_shop_sale_confirm_menu_script, g_world_input_primary_repeat, g_world_thread_task_active);
    world_shop_run_obtain_gil_menu();
}
