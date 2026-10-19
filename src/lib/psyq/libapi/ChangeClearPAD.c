/* 0x800220f4: BIOS 0xb0:0x5b tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's ChangeClearPAD (table B, 0x5b): the same choice for the BIOS's pad and memory card handler,
 * which acknowledges the VBlank interrupt itself unless `mode` is 0 (psx-spx). PadInit, InitCARD, StartCARD and
 * main_card_init_events set 0; setIntr follows the VBlank callback as it does for ChangeClearRCnt. */
void ChangeClearPAD(s32 mode) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_CHANGE_CLEAR_PAD);
}
