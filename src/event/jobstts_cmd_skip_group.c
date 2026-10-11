#include "fft/event_jobstts.h"
#include "psx/types.h"

/* Skip a command plus, for group opcodes 1 and 2, the child commands it counts;
 * return the command that follows. */
menu_instruction_t* jobstts_cmd_skip_group(menu_instruction_t* data) {
    s32 count;

    if (data->header.opcode == MENU_SCRIPT_OP_IF_GROUP) {
        count = data->conditional.count + 1;
    } else if (data->header.opcode == MENU_SCRIPT_OP_IF_ELSE_GROUP) {
        count = data->if_else.count_true + data->if_else.count_false + 1;
    } else {
        count = 1;
    }

    while (--count != -1) {
        data = menu_next_instruction(data);
    }
    return data;
}
