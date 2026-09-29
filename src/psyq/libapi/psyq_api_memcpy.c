/* 0x80026f94: BIOS 0xa0:0x2a tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libgpu.h"

void psyq_api_memcpy(void* destination, const void* source, int size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_MEMCPY);
}
