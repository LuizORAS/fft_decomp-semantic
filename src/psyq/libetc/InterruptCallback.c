#include "psx/libetc.h"

void* InterruptCallback(s32 channel, psyq_interrupt_callback_t callback) {
    return g_psyq_etc_dispatch->irq_callback(channel, callback);
}
