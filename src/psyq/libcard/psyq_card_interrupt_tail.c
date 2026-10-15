#include "psx/card_interrupt_abi.h"
#include "psx/cpu_abi_inline.h"

/* The interrupt path _patch_card hooks into the BIOS: when the controller port's interrupt is pending, wait
 * until the port's status clears and jump to the saved continuation; otherwise return. */
void psyq_card_interrupt_tail(void) {
    /* BIOS enters with its live base in v1 and returns through the saved continuation. */
    psyq_card_interrupt_result = psyq_card_interrupt_base->interrupt_mask;
    PSYQ_CPU_LOAD_WAIT();
    psyq_card_interrupt_result &= PSYQ_CARD_IRQ_PENDING_MASK;
    if (psyq_card_interrupt_result == 0)
        goto interrupt_not_pending;
wait_clear:
    psyq_card_interrupt_result = psyq_card_interrupt_base->joy_status;
    PSYQ_CPU_LOAD_WAIT();
    psyq_card_interrupt_result &= PSYQ_CARD_IRQ_PENDING_MASK;
    if (psyq_card_interrupt_result != 0)
        goto wait_clear;
    psyq_card_interrupt_result = (u32)g_psyq_card_patch_continuation;
    PSYQ_CPU_LOAD_WAIT();
    goto*(void*)psyq_card_interrupt_result;
interrupt_not_pending:
}
