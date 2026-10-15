#include "psx/bios.h"
#include "psx/gte_inline.h"
#include "psx/libapi.h"
#include "psx/libgte.h"

/* Copy LIBGTE's 14-word template over the start of the BIOS exception handler (entry 6 of the BIOS's C0
 * table), in a critical section, then flush the instruction cache. The template is psx-spx's
 * "patch_missing_cop0r13_in_exception_handler": it adds the read of the cause register that the oldest BIOS
 * (CEX-1000 KT-3) misses, so an interrupt that arrives with a GTE command does not run the command twice.
 * InitGeom calls it. */
void _patch_gte(void) {
    /* The copy loop retains the handwritten word and template-bound registers. */
    psyq_bios_c0_table_t* table;
    u32* destination;
    register u32 word __asm__("$3");
    register u32* source __asm__("$10");
    register u32* end __asm__("$9");
    PSYQ_CPU_GLOBAL_SAVE_RA(g_psyq_gte_patch_saved_ra);
    PSYQ_CPU_GLOBAL_CALL(EnterCriticalSection);
    PSYQ_GTE_BIOS_TABLE(table);
    PSYQ_CPU_ADDRESS_HIGH(source, g_psyq_gte_exception_patch_template);
    PSYQ_CPU_ADDRESS_HIGH(end, g_psyq_gte_exception_patch_template_end);
    destination = table->exception_handler;
    PSYQ_CPU_ADDRESS_LOW(source, g_psyq_gte_exception_patch_template);
    PSYQ_CPU_ADDRESS_LOW(end, g_psyq_gte_exception_patch_template_end);
    do {
        word = *source++;
        destination++;
        destination[-1] = word;
    } while (source != end);
    PSYQ_CPU_GLOBAL_CALL(FlushCache);
    PSYQ_CPU_GLOBAL_CALL(ExitCriticalSection);
    PSYQ_CPU_GLOBAL_RESTORE_RA(g_psyq_gte_patch_saved_ra);
}
