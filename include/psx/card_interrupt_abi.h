#ifndef PSX_CARD_INTERRUPT_ABI_H
#define PSX_CARD_INTERRUPT_ABI_H

#include "psx/libcard.h"

/* Reserved architectural state; declarations precede headers with C inline definitions. */
register volatile psyq_card_interrupt_view_t* psyq_card_interrupt_base __asm__("$3");
register u32 psyq_card_interrupt_result __asm__("$2");
register void* psyq_card_return_address __asm__("$31");

#endif
