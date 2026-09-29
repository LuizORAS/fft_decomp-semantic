#include "fft/event_card.h"
#include "psx/libapi.h"
#include "psx/types.h"

s32 card_io_format_selected_slot(void) {
    s32 result;
    char path[8];

    result = card_io_wait_operation_result(10);
    if (result > CARD_IO_RESULT_COMPLETE)
        return 0;
    if (g_card_io_selected_slot == 0)
        strcpy(&path[0], g_card_file_slot_0_path);
    else
        strcpy(&path[0], g_card_file_slot_1_path);
    result = format(&path[0]);
    result = format(&path[0]);
    result = format(&path[0]);
    return result;
}
