/* 0x8002229c: BIOS 0xa0:0x13 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's setjmp (table A, 0x13): save the return address, stack pointer, frame pointer, s0-s7 and gp in
 * `jump_buffer` and return 0; resuming that buffer returns here again with a nonzero value (psx-spx).
 * startIntr records the state HookEntryInt resumes after each exception, which enters trapIntr. */
s32 setjmp(jmp_buf jump_buffer) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_SETJMP);
}
