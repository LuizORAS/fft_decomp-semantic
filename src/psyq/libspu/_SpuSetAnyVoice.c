/* SCUS_942.21 0x80019b80..0x80019d87. */
#include "psx/libspu.h"

u32 _SpuSetAnyVoice(s32 on_off, u32 voice_bit, s32 reg_index_low, s32 reg_index_high) {
    s32 operation = on_off;
    u32 voices = voice_bit;
    u32 current;
    /* This view keeps queued-state updates after their mask-register writes. */
    extern volatile u32 queued_flags __asm__("_spu_RQmask");
    {
        /* The high/low pins preserve the retail v1/v0 register reads. */
        volatile u16* registers;
        register u32 high __asm__("$3");
        register u32 low __asm__("$2");
        if (_spu_env & 1) {
            registers = g_psyq_spu_queued_register_base;
        } else {
            registers = (u16*)_spu_RXX;
        }
        high = registers[reg_index_high];
        /* The tie prevents the high word from merging into the result register. */
        __asm__("" : "=r"(high) : "0"(high));
        low = registers[reg_index_low];
        /* The tie prevents index scaling from being retained across the switch. */
        __asm__ volatile("" : "=r"(reg_index_low), "=r"(reg_index_high) : "0"(reg_index_low), "1"(reg_index_high));
        current = low | ((high & 0xff) << 16);
    }
    switch (operation) {
    case 1: {
        /* A typed array index recomputes the retail shared low-register byte offset. */
        s32 low_offset = reg_index_low << 1;
        if (_spu_env & 1) {
            volatile u16* registers = g_psyq_spu_queued_register_base;
            volatile u16* low = (u16*)((u8*)registers + low_offset);
            volatile u16* high = &registers[reg_index_high];
            u32 bits = *low;
            bits |= voices;
            *low = bits;
            *high |= (voices >> 16) & 0xff;
            {
                u32 flags = operation << ((reg_index_low - PSYQ_SPU_REG_KEY_OFF_LOW) >> 1);
                queued_flags |= flags;
            }
        } else {
            /* The bank and low-word pins preserve the retail a1/v0 registers. */
            register volatile u16* registers __asm__("$5") = (u16*)_spu_RXX;
            volatile u16* low = (u16*)(low_offset + (u32)registers);
            u32 high_offset = reg_index_high << 1;
            volatile u16* high;
            register u32 bits __asm__("$2");
            /* This tie keeps the high-index shift in the register-pointer load delay. */
            __asm__("" : "=r"(high_offset) : "0"(high_offset));
            bits = *low;
            /* The input dependence keeps address addition after the low-word read. */
            __asm__ volatile("" : "=r"(high_offset) : "0"(high_offset), "r"(bits));
            /* Pointer arithmetic reverses the retail integer-add operand order. */
            high = (u16*)(high_offset + (u32)registers);
            bits |= voices;
            *low = bits;
            /* The tie prevents destructive reuse of the bank pointer for this address. */
            __asm__("" : "=r"(high) : "0"(high));
            *high |= (voices >> 16) & 0xff;
        }
        {
            u32 mask = voices & 0xffffff;
            current |= mask;
        }
        break;
    }
    case 0: {
        /* The offset pin preserves the retail v1 low-register address calculation. */
        register s32 low_offset __asm__("$3") = reg_index_low << 1;
        if (_spu_env & 1) {
            volatile u16* registers = g_psyq_spu_queued_register_base;
            volatile u16* low = (u16*)((u8*)registers + low_offset);
            u32 bits = *low;
            volatile u16* high;
            bits &= ~voices;
            high = &registers[reg_index_high];
            *low = bits;
            *high &= ~((voices >> 16) & 0xff);
            queued_flags |= 1 << ((reg_index_low - PSYQ_SPU_REG_KEY_OFF_LOW) >> 1);
        } else {
            /* The clear-mask and high-offset pins preserve retail reuse of a0. */
            volatile u16* registers = (u16*)_spu_RXX;
            volatile u16* low = (u16*)(low_offset + (u32)registers);
            u32 bits = *low;
            register u32 clear __asm__("$4");
            register u32 high_offset __asm__("$4");
            volatile u16* high;
            clear = ~voices;
            bits &= clear;
            high_offset = reg_index_high << 1;
            /* Pointer arithmetic reverses the retail integer-add operand order. */
            high = (u16*)(high_offset + (u32)registers);
            *low = bits;
            *high &= ~((voices >> 16) & 0xff);
        }
        {
            u32 mask = voices & 0xffffff;
            current &= ~mask;
        }
        break;
    }
    }
    return current & 0xffffff;
}
