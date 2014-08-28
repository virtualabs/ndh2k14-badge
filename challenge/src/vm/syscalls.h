#ifndef __INC_VM_SYSCALLS_H
#define __INC_VM_SYSCALLS_H

#include "vm.h"

#define VM_SYS_READ     0x52
#define VM_SYS_WRITE    0x57
#define VM_SYS_LED      0x4C

void vm_sys_read(PVMContext pVMContext);
void vm_sys_write(PVMContext pVMContext);
void vm_sys_led(PVMContext pVMContext);

#endif /* __INC_VM_SYSCALLS_H */
