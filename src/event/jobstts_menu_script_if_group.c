#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Run the next `count` commands only if condition data->conditional.predicate
 * returns nonzero for the row (the literal data->conditional.value, or the row
 * being drawn while a list row is rendered); otherwise skip them. */
menu_instruction_t* jobstts_menu_script_if_group(menu_instruction_t* data) {
    s32 count;
    jobstts_menu_script_condition_t condition;
    s32 index;

    count = data->conditional.count;
    condition = g_jobstts_menu_script_conditions[data->conditional.predicate];
    if (g_jobstts_menu_list_row_rendering == 0) {
        index = data->conditional.value;
    } else {
        index = g_jobstts_menu_scroll_base_index + g_jobstts_menu_list_row_index;
        if (g_jobstts_menu_list_scroll_offset_y < 0) {
            index -= 1;
        }
    }
    data = menu_next_instruction(data);
    if (condition(index) == 0) {
        count -= 1;
        while (count != -1) {
            data = menu_next_instruction(data);
            count -= 1;
        }
        return data;
    }
    count -= 1;
    while (count != -1) {
        data = g_jobstts_menu_script_handlers[data->header.opcode](data);
        count -= 1;
    }
    return data;
}
