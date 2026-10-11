#include "fft/event_equip.h"
#include "psx/types.h"

/* Run the next `then_count` commands and skip the following `else_count`
 * when row callback command->if_else.predicate returns nonzero; otherwise skip the first
 * group and run the second. The callback's argument is command->if_else.value, or the row
 * being drawn while a list row is rendered. */
menu_instruction_t* equip_cmd_run_if_else_group_handler(menu_instruction_t* command) {
    s32 then_count;
    s32 else_count;
    s32 (*handler)(s32);
    s32 idx;

    then_count = command->if_else.count_true;
    else_count = command->if_else.count_false;
    handler = g_equip_menu_list_row_callbacks[command->if_else.predicate];
    if (g_equip_menu_list_row_mode == 0) {
        idx = command->if_else.value;
    } else {
        idx = g_equip_menu_scroll_base_index + g_equip_menu_list_row_index;
        if (g_equip_menu_list_scroll_offset < 0) {
            idx--;
        }
    }
    command = menu_next_instruction(command);
    if (handler(idx) != 0) {
        for (then_count--; then_count != -1; then_count--) {
            command = g_equip_cmd_handlers[command->header.opcode](command);
        }
        for (else_count--; else_count != -1; else_count--) {
            command = menu_next_instruction(command);
        }
    } else {
        for (then_count--; then_count != -1; then_count--) {
            command = menu_next_instruction(command);
        }
        for (else_count--; else_count != -1; else_count--) {
            command = g_equip_cmd_handlers[command->header.opcode](command);
        }
    }
    return command;
}
