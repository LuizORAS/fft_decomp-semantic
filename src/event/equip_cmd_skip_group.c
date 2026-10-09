#include "fft/event_equip.h"
#include "psx/types.h"

u8* equip_cmd_skip_group(u8* command) {
    s32 count;

    if (command[0] == MENU_SCRIPT_OP_IF_GROUP) {
        count = command[4] + 1;
    } else if (command[0] == MENU_SCRIPT_OP_IF_ELSE_GROUP) {
        count = command[4] + command[5] + 1;
    } else {
        count = 1;
    }
    count -= 1;
    while (count != -1) {
        command += command[1];
        count -= 1;
    }
    return command;
}
