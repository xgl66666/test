#ifndef CYGONCE_HAL_HAL_PRIVATE_MISC_H
#define CYGONCE_HAL_HAL_PRIVATE_MISC_H

// For Thread Stack Information Trace use
typedef struct {
    cyg_uint16        TaskID;
    char              *Name;
    CYG_ADDRESS       StackBase;
    cyg_uint32        StackSize;
    cyg_uint32        StackReg;
    cyg_uint32        StackUsedAddr;
    cyg_uint32        StackSearchAddrLong;
    cyg_uint32        StackSearchAddrShort;
    cyg_uint32        UsedSize;
    cyg_uint32        UsedSizePercentage;
    bool              Stored;
} Task_Stack_Info;

typedef struct {
    //cyg_uint16        TaskID;
    char              *Name;
    cyg_uint32        count;
} Task_Usage_Info;

#endif
