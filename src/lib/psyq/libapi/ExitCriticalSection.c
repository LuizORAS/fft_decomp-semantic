#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* Enable interrupts again through BIOS system call 2. */
void ExitCriticalSection(void) {
    PSYQ_BIOS_SYSCALL(PSYQ_BIOS_SYSCALL_EXIT_CRITICAL_SECTION);
}
