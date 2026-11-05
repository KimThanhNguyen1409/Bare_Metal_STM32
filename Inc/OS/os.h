#ifndef OS_H
#define OS_H
#include <stdint.h>
typedef struct
{
    uint32_t *sp; // Stack pointer of current task
} TCB_t;
#endif