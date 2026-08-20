#include "os.h"
// Read PM of STM32F108 for more information of these resigters
#define STK_CTRL *((volatile uint32_t*)0xE000E010)
#define STK_LOAD *((volatile uint32_t*)0xE000E014)
#define STK_VAL *((volatile uint32_t*)0xE000E018)
#define SCB_ICSR *((volatile uint32_t*)0xE000ED04)
void OS_TaskCreate(TCB_t* tcb, void (*task_func)(void*), void* arg, uint32_t* stack_base, uint32_t stack_size)
{
    // Cortex-M is a full descending architecture
    // Therefore, the top pointer must point to the last element of the array
    uint32_t* sp = stack_base + stack_size; // Starting at the highest
    // Stack of hardware, auto PUSH/POP
    sp--;
    *sp = 0x01000000; // xPSR: Must set bit 24 to 1 (Thumb state), otherwise it will return HardFault
    sp--;
    *sp = (uint32_t)task_func; // PC: Program Counter point to the task function
    sp--;
    *sp = 0x00000000; // LR: Link Resigter
    sp--;
    *sp = 0x12121212; // R12
    sp--;
    *sp = 0x03030303; // R3
    sp--;
    *sp = 0x02020202; // R2
    sp--;
    *sp = 0x01010101; // R1
    sp--;
    *sp = (uint32_t)arg; // R0
    // Stack of software (PendSV), PUSH/POP by itself
    sp--;
    *sp = 0x11111111; // R11
    sp--;
    *sp = 0x10101010; // R10
    sp--;
    *sp = 0x09090909; // R9
    sp--;
    *sp = 0x08080808; // R8
    sp--;
    *sp = 0x07070707; // R7
    sp--;
    *sp = 0x06060606; // R6
    sp--;
    *sp = 0x05050505; // R5
    sp--;
    *sp = 0x04040404; // R4

    tcb->sp         = sp; // Store the current pointer to TCB
    tcb->sleep_time = 0;  // Intialize sleep time = 0
}
TCB_t* volatile current_task;
TCB_t* volatile next_task;
TCB_t* head = 0;
TCB_t* tail = 0;
// This function will mainly add task into a cicrlar linked lists
void OS_AddThread(TCB_t* tcb)
{
    tcb->sleep_time = 0; // Intialize sleep time = 0
    __asm volatile("CPSID I \n");
    if (head == 0)
    {
        head      = tcb;
        tail      = tcb;
        tcb->next = tcb;
    }
    else
    {
        tail->next = tcb;
        tcb->next  = head;
        tail       = tcb;
    }
    __asm volatile("CPSIE I \n");
}
// This function generate SysTick. The value "ticks" is CPU clock speed between each interrupt
// For instance, CPU clock speed is 16MHz, if you want interrupt in 1ms -> ticks = 16000000 / 1000 = 16000
void OS_InitSysTick(uint32_t ticks)
{
    STK_LOAD = ticks - 1; // Set the reversed counter
    STK_VAL  = 0;         // Reset the current counter = 0
    // Enable Bit 0 ENABLE - Bit 1 TICKINT - Bit 2 CLKSOURCE
    // Read PM at page 247 for more info
    STK_CTRL |= (1 << 0) | (1 << 1) | (1 << 2);
}
void Systick_Handler(void)
{
    TCB_t* temp = head; // Initilize varible for the search
    // We will look the task that have sleep time and decline it by 1ms
    if (temp != 0)
    {
        do
        {
            if (temp->sleep_time > 0)
                temp->sleep_time--;
            temp = temp->next;
        } while (temp != head);
    }
    // After that, we will look for the task (or next task) that are awake
    next_task = current_task;
    do
    {
        next_task = next_task->next;
        if (next_task->sleep_time == 0)
            break;
    } while (next_task != current_task);
    // Set bit 28 to 1 -> enable PENDSVSET. In this function, it will "kick" the old task and add the new task
    SCB_ICSR |= (1 << 28);
}
__attribute__((naked)) void PendSV_Handler(void)
{
    __asm volatile(
        "CPSID I \n"     // Disable interrupt
        "MRS R0, PSP \n" // Read the top of the current stack of task A (PSP) and then put into R0
        /*=== STORE TASK A ===*/
        "LDR R1, =current_task \n" // Take the address of the current task
        "LDR R2, [R1] \n"          // Take R1's content, which is TCB of task A

        "CBZ R2, restore_context \n" // Compare and Branch on Zero: If TCB_A is NULL (first context switch), jump
                                     // straight to restore Task B

        "STMDB R0!, {R4-R11} \n" /* STMDB: Store Multiple Decrement before. It takes values from
                                    R4 to R11 then stores into SRAM which R0 points to. "!" means
                                    after storing, R0 will automatically descend to the new place */
        "STR R0, [R2] \n"        // Store R0 into the first stack of TCB_A (TCB->sp)
        /*=== SWITCH TASK ===*/
        "restore_context: \n"
        "LDR R3, =next_task \n" // Take the address of next_task
        "LDR R4, [R3] \n"       // Take R3's content, which is TCB of task B
        "STR R4, [R1] \n" // Overwrite the current task with Task B. Right now, the system knows that Task B is running
        /*=== RESTORE TASK B ===*/
        "LDR R0, [R4] \n" // Read the address of tcb->sp of Task B then put it into R0. R0 is currently pointing to the
                          // top of Task B's stack
        "LDMIA R0!, {R4-R11} \n" /* LDMIA (Load Multiple Increment After). It takes the data from SRAM and
                                    stores into R4 to R11 of CPU. "!" makes R0 go up */
        "MSR PSP, R0 \n"         // Help CPU notice that stack pointer of task B is in R0

        "CPSIE I \n" // Enable interrupt
        "BX LR \n"   // Branch Exchange
    );
}
__attribute__((naked)) void SVCall_Handler(void)
{
    __asm volatile("LDR R0, =current_task \n" // Take the current task's address
                   "LDR R1, [R0] \n"          // Take TCB of the first task
                   "LDR R0, [R1] \n"          // Take the Stack Pointer of this task (tcb->sp)

                   "LDMIA R0!, {R4-R11} \n" // Load data from SRAM and put into 8 registers (R4-R11)
                   "MSR PSP, R0 \n"         // Update the current stack's top into PSP regsiter

                   "ORR LR, LR, #0x04 \n" // Set bit 2 to 1 in LR (Link Register) register

                   "CPSIE I \n" // Enable interrupt
                   "BX LR \n"   // Branch Exchange
    );
}
void OS_Start(void)
{
    if (head != 0)
    {
        current_task = head;
    }
    // Ticks = (System Clock / Tick Rate) - 1
    OS_InitSysTick(8000);
    __asm volatile("SVC 0 \n");
    while (1)
        ;
}
void OS_Delay(uint32_t ticks)
{
    // Lock the interrupt for safety
    __asm volatile("CPSID I\n");
    current_task->sleep_time = ticks;
    next_task                = current_task;
    do
    {
        next_task = next_task->next;
        if (next_task->sleep_time == 0)
        {
            break;
        }
    } while (next_task != current_task);
    // Open it back
    __asm volatile("CPSIE I\n");
    SCB_ICSR |= (1 << 28);
}