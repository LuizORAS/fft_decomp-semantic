#include "fft/event_equip.h"
#include "psx/pad.h"
#include "psx/types.h"

void equip_menu_text_entry_wait_thread(void) {
    world_menu_entry_t* thread;
    world_menu_text_binding_t* task;
    u32* input;
    u16 text_id;
    u16 header_id;
    u16 child_id;
    u16 task_arg;

    thread = (world_menu_entry_t*)g_battle_threads[g_battle_current_thread_id].function_parameter_1;
    thread->text_binding->prepare();

    input = battle_script_get_controller_input_pointer(0);
    text_id = thread->text_id;
    task = thread->text_binding;
    header_id = thread->header_id;
    g_equip_input_controller = input;
    child_id = task->text_thread_id;
    task_arg = task->speaker_id;
    *input = PSX_PAD_CIRCLE;
    battle_menu_handle_action(thread, 0);

    battle_thread_start(child_id, battle_text_character_handling_thread);
    battle_thread_set_four_parameters(child_id, header_id, text_id, task_arg, task_arg);
    battle_thread_wait_until_inactive(g_battle_current_thread_id - 1);
    battle_thread_wait_until_inactive(child_id);
    battle_thread_exit_current();
}
