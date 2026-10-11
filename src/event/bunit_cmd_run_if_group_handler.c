#include "fft/event_bunit.h"
#include "psx/types.h"

/* Run the following group if its predicate succeeds for the literal or
 * current list row; otherwise skip it. */
menu_instruction_t* bunit_cmd_run_if_group_handler(menu_instruction_t* cmd) {
    s32 (*callback)(s32);
    s32 count;
    s32 row;

    callback = g_bunit_menu_row_callbacks[cmd->conditional.predicate];
    count = cmd->conditional.count;
    if (g_bunit_menu_scroll_list_active == 0) {
        row = cmd->conditional.value;
    } else {
        row = g_bunit_menu_scroll_base_index + g_bunit_menu_cursor_row;
        if (g_bunit_menu_scroll_pixel_offset < 0) {
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
            cmd = g_bunit_cmd_handlers[cmd->header.opcode](cmd);
        }
    }
    return cmd;
}
