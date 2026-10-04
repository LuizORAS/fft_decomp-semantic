#ifndef PSX_LIBCARD_H
#define PSX_LIBCARD_H

#include "psx/bios.h"
#include "psx/libapi.h"

/* Original resident card wrappers and BIOS patch helpers. */
void _patch_card(void);
void _patch_card2(void);
void psyq_card_restore_exception_prefix(void);

/* The BIOS supplies the live v1 base. The /ACK wait on bit 7 of +0x1044
 * (psx-spx early_card_irq_patch: JOY_STAT.7) puts it at the I/O base
 * 0x1f800000. */
typedef struct {
    u8 _unknown_0000[0x1044];
    u32 joy_status; /* 0x1044: JOY_STAT; bit 7 is the /ACK input level */
    u8 _unknown_1048[0x2c];
    u32 interrupt_mask; /* 0x1074: I_MASK by the same base; psx-spx labels this load I_STAT.7 */
} psyq_card_interrupt_view_t;

typedef struct {
    void* _unknown_00[6];
    u32* exception_handler;
} psyq_card_bios_c0_table_t;

typedef struct {
    void* _unknown_000[PSYQ_BIOS_B_CHANGE_CLEAR_PAD];
    u32* change_clear_pad;
} psyq_card_bios_b0_table_t;

enum {
    PSYQ_CARD_IRQ_PENDING_MASK = 0x80,
    PSYQ_CARD_EXCEPTION_ADDRESS_HIGH_WORD = 0x70 / 4,
    PSYQ_CARD_EXCEPTION_ADDRESS_LOW_WORD = 0x74 / 4,
    PSYQ_CARD_INTERRUPT_PATCH_WORD = 0x28 / 4,
    PSYQ_CARD_PAD_DELAY_PATCH_WORD = 0x9c8 / 4,
    PSYQ_CARD_EXCEPTION_RESTORE_WORD = 0x70 / 4,
};

extern u32 g_psyq_card_patch_saved_ra;
extern u32 g_psyq_card_restore_saved_ra;
extern void* g_psyq_card_patch_continuation;
extern u32 g_psyq_card_interrupt_jump_template[];
extern u32 g_psyq_card_interrupt_jump_template_end[];
extern u32 g_psyq_card_delay_template[];
extern u32 g_psyq_card_delay_template_end[];
extern u32 g_psyq_card_restore_template[];
extern u32 g_psyq_card_restore_template_end[];

void psyq_card_interrupt_tail(void);
void psyq_card_short_delay(void);
void psyq_card_long_delay(void);

#endif
