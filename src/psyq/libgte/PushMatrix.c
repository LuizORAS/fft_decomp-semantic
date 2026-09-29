#include "psx/cpu_return_address_abi.h"
#include "psx/gte_inline.h"
#include "psx/libc.h"
#include "psx/libgte.h"

/* main 0x8001cb04–0x8001cba4: save the eight rotation/translation controls. */
void PushMatrix(void) {
    /* Fixed scratch registers reproduce the handwritten COP2/ABI allocation. */
    register s32 offset __asm__("$14");
    register psyq_packed_matrix_t* frame __asm__("$15");
    register s32 first __asm__("$8");
    register s32 second __asm__("$9");
    register s32 third __asm__("$10");
    register u32 save_page __asm__("$1");
    register s32 available __asm__("$1");
    offset = g_psyq_gte_matrix_stack_offset;
    available = offset < 640;
    if (available)
        goto save_frame;
    save_page = PSYQ_GTE_MATRIX_STACK_RA_PAGE;
    PSYQ_GTE_GLOBAL_SAVE_RA_PAGE(save_page, g_psyq_gte_matrix_stack_saved_ra);
    PSYQ_GTE_GLOBAL_PRINTF(printf, g_psyq_gte_matrix_stack_overflow_message);
    PSYQ_CPU_GLOBAL_RESTORE_RA(g_psyq_gte_matrix_stack_saved_ra);
    goto* psyq_cpu_return_address;
save_frame:
    PSYQ_CPU_ADDRESS_HIGH(frame, g_psyq_gte_matrix_stack);
    frame = (psyq_packed_matrix_t*)((u32)frame + offset);
    PSYQ_CPU_ADDRESS_LOW(frame, g_psyq_gte_matrix_stack);
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_R11_R12, first);
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_R13_R21, second);
    frame->words[0] = first;
    frame->words[1] = second;
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_R22_R23, first);
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_R31_R32, second);
    frame->words[2] = first;
    frame->words[3] = second;
    PSYQ_GTE_CFC2_NOP(PSYQ_GTE_CTRL_R33, first);
    frame->words[4] = first;
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_TRX, first);
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_TRY, second);
    PSYQ_GTE_CFC2(PSYQ_GTE_CTRL_TRZ, third);
    frame->words[5] = first;
    frame->words[6] = second;
    frame->words[7] = third;
    PSYQ_CPU_TRAP_ADDI(offset, offset, 32);
    g_psyq_gte_matrix_stack_offset = offset;
    goto* psyq_cpu_return_address;
}
