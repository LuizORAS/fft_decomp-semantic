#include "psx/libetc.h"

int ResetCallback(void) {
    return g_psyq_etc_dispatch->reset();
}
