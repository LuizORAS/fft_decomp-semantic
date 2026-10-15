#include "fft/event_bunit.h"
#include "psx/types.h"

/* Skip a command plus, for group opcodes 1 and 2, the child commands it counts;
 * return the command that follows. */
u8* bunit_menu_script_skip_group(u8* command) {
    s32 count;

    if (command[0] == MENU_SCRIPT_OP_IF_GROUP) {
        count = command[4] + 1;
    } else if (command[0] == MENU_SCRIPT_OP_IF_ELSE_GROUP) {
        count = command[4] + command[5] + 1;
    } else {
        count = 1;
    }

    while (--count != -1) {
        command += command[1];
    }
    return command;
}
