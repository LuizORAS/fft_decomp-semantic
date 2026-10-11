#include "fft/event_equip.h"
#include "psx/types.h"

/* Run the next `count` commands only if row callback command->conditional.predicate returns
 * nonzero for the row (literal command->conditional.value, or the row being drawn while a
 * list row is rendered); otherwise skip them. */
menu_instruction_t* equip_cmd_run_if_group_handler(menu_instruction_t* command) {
    s32 (*handler)(s32);
    s32 count;
    s32 arg;

    handler = g_equip_menu_list_row_callbacks[command->conditional.predicate];
    count = command->conditional.count;
    if (g_equip_menu_list_row_mode == 0) {
        arg = command->conditional.value;
    } else {
        arg = g_equip_menu_scroll_base_index + g_equip_menu_list_row_index;
        if (g_equip_menu_list_scroll_offset < 0) {
            arg = arg - 1;
        }
    }

    command = menu_next_instruction(command);
    if (handler(arg) == 0) {
        count--;
        while (count != -1) {
            command = menu_next_instruction(command);
            count--;
        }
        return command;
    }
    count--;
    while (count != -1) {
        command = g_equip_cmd_handlers[command->header.opcode](command);
        count--;
    }
    return command;
}
