#include "os.h"
// Read PM of STM32F108 for more information of these resigters
#define STK_CTRL *((volatile uint32_t *)0xE000E010)
#define STK_LOAD *((volatile uint32_t *)0xE000E014)
#define STK_VAL *((volatile uint32_t *)0xE000E018)
#define SCB_ICSR *((volatile uint32_t *)0xE000ED04)
void OS_TaskCreate(TCB_t *tcb, void (*task_func)(void), uint32_t *stack_base, uint32_t stack_size)
{
    // Cortex-M is a full descending architecture
    // Therefore, the top pointer must point to the last element of the array
    uint32_t *sp = stack_base + stack_size; // Starting at the highest
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
    *sp = 0x00000000; // R0
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

    tcb->sp = sp; // Store the current pointer to TCB
}
TCB_t *current_task;
TCB_t *next_task;
// This function generate SysTick. The value "ticks" is CPU clock speed between each interrupt
// For instance, CPU clock speed is 16MHz, if you want interrupt in 1ms -> ticks = 16000000 / 1000 = 16000
void OS_InitSysTick(uint32_t ticks)
{
    STK_LOAD = ticks - 1; // Set the reversed counter
    STK_VAL = 0;          // Reset the current counter = 0
    // Enable Bit 0 ENABLE - Bit 1 TICKINT - Bit 2 CLKSOURCE
    // Read PM at page 247 for more info
    STK_CTRL |= (1 << 0) | (1 << 1) | (1 << 2);
}
void SysTick_Handler(void)
{
    // Set bit 28 to 1 -> enable PENDSVSET
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

        "CBZ R2, restore_context \n" // Compare and Branch on Zero: If TCB_A is NULL (first context switch), jump straight to restore Task B

        "STMDB R0!, {R4-R11} \n" /* STMDB: Store Multiple Decrement before. It takes values from
                                    R4 to R11 then stores into SRAM which R0 points to. "!" means
                                    after storing, R0 will automatically descend to the new place */
        "STR R0, [R2] \n"        // Store R0 into the first stack of TCB_A (TCB->sp)
        /*=== SWITCH TASK ===*/
        "restore_context: \n"
        "LDR R3, =next_task \n" // Take the address of next_task
        "LDR R4, [R3] \n"       // Take R3's content, which is TCB of task B
        "STR R4, [R1] \n"       // Overwrite the current task with Task B. Right now, the system knows that Task B is running
        /*=== RESTORE TASK B ===*/
        "LDR R0, [R4] \n"        // Read the address of tcb->sp of Task B then put it into R0. R0 is currently pointing to the top of Task B's stack
        "LDMIA R0!, {R4-R11} \n" /* LDMIA (Load Multiple Increment After). It takes the data from SRAM and
                                    stores into R4 to R11 of CPU. "!" makes R0 go up */
        "MSR PSP, R0 \n"         // Help CPU notice that stack pointer of task B is in R0 (SỬA Ở ĐÂY: MSR thay vì MSP)

        "CPSIE I \n" // Enable interrupt
        "BX LR \n"   // Branch Exchange
    );
}
__attribute__((naked)) void SVC_Handler(void)
{
    __asm volatile(
        "LDR R0, =current_task \n" // Take the current task's address
        "LDR R1, [R0] \n"          // Take TCB of the first task
        "LDR R0, [R1] \n"          // Take the Stack Pointer of this task (tcb->sp)

        "LDMIA R0!, {R4-R11} \n" // Take data from 8 registers (R4-R11) and put into SRAM
        "MSR PSP, R0 \n"         // Update the current stack's top into PSP regsiter

        "ORR LR, LR, #0x04 \n" // Set bit 2 to 1 in LR(Link Register) register

        "CPSIE I \n"
        "BX LR \n");
}
void OS_Start(void)
{
    // Ticks = (System Clock / Tick Rate) - 1
    OS_InitSysTick(72000);
    __asm volatile("SVC 0 \n");
    while (1)
        ;
}