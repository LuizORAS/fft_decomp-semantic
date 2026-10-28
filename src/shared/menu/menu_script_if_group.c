#include "fft/menu_script.h"

/* Run the next `count` commands only if the row callback cmd->conditional.predicate
 * selects returns nonzero for the row (the literal cmd->conditional.value, or the
 * scroll cursor row when the scroll list is active); otherwise skip them. */
menu_instruction_t* menu_script_if_group(menu_instruction_t* cmd) {
    s32 (*callback)(s32);
    s32 count;
    s32 row;

    callback = g_menu_row_callbacks[cmd->conditional.predicate].value;
    count = cmd->conditional.count;
    if (g_menu_scroll_list_active == 0) {
        row = cmd->conditional.value;
    } else {
        row = g_menu_scroll_base_index + g_menu_list_row_index;
        if (g_menu_list_scroll_offset < 0) {
            row -= 1;
        }
    }
    cmd = menu_next_instruction(cmd);
    if (callback(row) == 0) {
        while (--count != -1) {
            cmd = menu_next_instruction(cmd);
        }
    } else {
        while (--count != -1) {
            cmd = menu_script_handlers[cmd->header.opcode](cmd);
        }
    }
    return cmd;
}
