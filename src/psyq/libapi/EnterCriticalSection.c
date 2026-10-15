#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* Disable interrupts through BIOS system call 1 (psx-spx: it clears status register bits 2 and 10). Library
 * code that changes interrupt or event state (InitCARD, SpuStart, SuzukiSPUInitialiser, main_card_init_events
 * and others) runs between it and ExitCriticalSection. */
void EnterCriticalSection(void) {
    PSYQ_BIOS_SYSCALL(PSYQ_BIOS_SYSCALL_ENTER_CRITICAL_SECTION);
}
