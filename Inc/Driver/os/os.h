#ifndef OS_H
#define OS_H

#include <stdint.h>

typedef struct tcb
{
    uint32_t* sp; // Stack pointer of current task
    struct tcb* next;
    uint32_t sleep_time; // Sleep time variable
} TCB_t;

void OS_Delay(uint32_t ticks);
void OS_TaskCreate(TCB_t* tcb, void (*task_func)(void*), void* arg, uint32_t* stack_base, uint32_t stack_size);
void OS_InitSysTick(uint32_t ticks);
void OS_AddThread(TCB_t* tcb);
void Systick_Handler(void);
__attribute__((naked)) void PendSV_Handler(void);
__attribute__((naked)) void SVCall_Handler(void);
void OS_Start(void);

#endif // OS_H