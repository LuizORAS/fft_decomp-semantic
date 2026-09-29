#include "psx/gte_inline.h"
#include "psx/libapi.h"
#include "psx/libgte.h"

/* One-function view of BIOS C0 dispatch; only entry6 is dereferenced. */
typedef struct {
    void* _unused_00[6];
    u32* exception_handler;
} psyq_bios_c0_table_t;

/* main 0x8001da68–0x8001dad0: replace the BIOS exception-handler prefix.
 * Critical-section/cache calls and BIOS dispatch remain actual interfaces. */
void _patch_gte(void) {
    /* The copy loop retains the handwritten word and template-bound registers. */
    register psyq_bios_c0_table_t* table __asm__("$2");
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
    __asm__ volatile("" : : "r"(destination));
    PSYQ_CPU_ADDRESS_LOW(source, g_psyq_gte_exception_patch_template);
    PSYQ_CPU_ADDRESS_LOW(end, g_psyq_gte_exception_patch_template_end);
    do {
        word = *source++;
        destination++;
        __asm__(""
            : "=r"(destination)
            : "0"(destination)); /* Keep the original increment before the previous-word store. */
        destination[-1] = word;
    } while (source != end);
    PSYQ_CPU_GLOBAL_CALL(FlushCache);
    PSYQ_CPU_GLOBAL_CALL(ExitCriticalSection);
    PSYQ_CPU_GLOBAL_RESTORE_RA(g_psyq_gte_patch_saved_ra);
}
