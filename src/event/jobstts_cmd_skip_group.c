#include "fft/event_jobstts.h"
#include "psx/types.h"

u8* jobstts_cmd_skip_group(u8* data) {
    s32 count;

    if (data[0] == MENU_SCRIPT_OP_IF_GROUP) {
        count = data[4] + 1;
    } else if (data[0] == MENU_SCRIPT_OP_IF_ELSE_GROUP) {
        count = data[4] + data[5] + 1;
    } else {
        count = 1;
    }

    while (--count != -1) {
        data += data[1];
    }
    return data;
}
