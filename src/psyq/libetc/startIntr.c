#include "psx/libetc.h"
psyq_interrupt_state_t* startIntr(void) {
    /* Reusing s0 for state and saved stack pointer preserves the final delay slot. */
    register psyq_interrupt_state_t* state __asm__("$16") = &g_psyq_etc_interrupt_state;
    if (state->initialized)
        return 0;
    {
        volatile u16* status = g_psyq_etc_irq_status;
        volatile u16* mask = g_psyq_etc_irq_mask;
        *mask = 0;
        *status = *mask;
    }
    *g_psyq_etc_dma_dpcr = PSYQ_DMA_DEFAULT_PRIORITY;
    psyq_etc_clear_interrupt_state_words((u32*)state, sizeof(*state) / sizeof(u32));
    if (setjmp(state->jump_buffer))
        trapIntr();
    {
        u32* stack_pointer = &g_psyq_etc_saved_stack_pointer;
        *stack_pointer = (u32)(stack_pointer
            + (sizeof(state->stack) / sizeof(u32) + sizeof(state->jump_buffer) / sizeof(u32) - 1
                - PSYQ_ETC_INTERRUPT_STACK_RESERVED_WORDS));
        HookEntryInt(stack_pointer - 1);
        /* Prevent folding the state store into a fresh absolute-address load. */
        __asm__("" : "=r"(stack_pointer) : "0"(stack_pointer));
        /* Recover the containing interrupt state from its saved-stack word. */
        ((psyq_interrupt_state_t*)((u8*)stack_pointer - (u32) & ((psyq_interrupt_state_t*)0)->jump_buffer[1]))
            ->initialized = 1;
        g_psyq_etc_dispatch->vblank_callback = (void* (*)(s32, void*))startIntrVSync();
        {
            psyq_callback_setter_t setter = startIntrDMA();
            /* The original callback-pointer store uses a0; an unpinned local selects v1. */
            register psyq_interrupt_dispatch_t* dispatch __asm__("$4") = g_psyq_etc_dispatch;
            dispatch->dma_callback = setter;
        }
        _96_remove();
        state = (psyq_interrupt_state_t*)((u8*)stack_pointer - (u32) & ((psyq_interrupt_state_t*)0)->jump_buffer[1]);
    }
    ExitCriticalSection();
    return state;
}
