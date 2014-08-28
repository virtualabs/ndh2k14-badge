#ifndef __INC_VM_H
#define __INC_VM_H

/**
 * Ndh2k14 embedded virtual machine
 * --------------------------------
 *
 *  Specifications
 *  --------------
 *
 *  - 32-bit RISC stack VM
 *  - obfuscated bytecode
 *  - emulated syscalls (IO)
 *  - basic logic & math operators
 *
 *  Bytecode
 *  --------
 *
 *  - 1 byte operation code
 *  - 1 byte mod
 *  - N bytes operand (depends on opcode)
 *  - fixed-size stack
 *  
 *  Flags
 *  -----
 *
 *  - ZF (zero) | CF (carry) | OF (overflow)
 *
 *  Operations
 *  ----------
 *
 *  arithmetics: ADD, SUB, DIV, MUL
 *  binary     : AND, OR, NOT, XOR
 *  syscall    : SYSCALL
 *  compar.    : CMP, TEST
 *  exec flow  : JMP, JNZ, JZ, JA, JB, RET
 *  stack      : PUSH, POP
 *
 *  Access modes
 *  ------------
 *
 *  - immediate (imm.)
 *  - indirect  (ind.)
 *  - stack-indexed (sti.)
 */

#define VM_FLAG_ZF      0x01
#define VM_FLAG_CF      0x02
#define VM_FLAG_OF      0x04
#define VM_FLAG_AF      0x08
#define VM_FLAG_BF      0x10

#define VM_MOD_IMM      0x4E
#define VM_MOD_IND      0x44
#define VM_MOD_STI      0x48

#define ERROR(vm)       (vm->flags & VM_FLAG_OF)
#define IS_ZERO(vm)     (vm->flags & VM_FLAG_ZF)
#define IS_ABOVE(vm)    (vm->flags & VM_FLAG_AF)
#define IS_BELOW(vm)    (vm->flags & VM_FLAG_BF)

#define VM_INST_ADD     0x4E
#define VM_INST_SUB     0x55
#define VM_INST_DIV     0x49
#define VM_INST_MUL     0x54
#define VM_INST_AND     0x44
#define VM_INST_OR      0x48
#define VM_INST_NOT     0x41
#define VM_INST_XOR     0x43
#define VM_INST_CMP     0x4B
#define VM_INST_TEST    0x56
#define VM_INST_JMP     0x4A
#define VM_INST_JNZ     0x52
#define VM_INST_JZ      0x4C
#define VM_INST_JA      0x42
#define VM_INST_JB      0x53
#define VM_INST_RET     0x32
#define VM_INST_PUSH    0x30
#define VM_INST_POP     0x31
#define VM_INST_SYSCALL 0x34
#define VM_INST_POPOFF  0x5B
#define VM_INST_LOADW   0x33
#define VM_INST_GETPC   0x5F
#define VM_INST_PSHOFF  0x5A

typedef struct {
    /* Main stack. */
    unsigned int stack[256];
    
    /* Instruction pointer. */
    unsigned char *pc;
    unsigned char *bytecode;
    
    /* Stack index. */
    int sti;

    /* Flags. */
    unsigned char flags;
} VMContext,*PVMContext;

void vm_init(PVMContext pVMContext, unsigned char *bytecode);
unsigned int vm_run(PVMContext pVMContext);
unsigned int vm_pop(PVMContext pVMContext);
void vm_push(PVMContext pVMContext, unsigned int value);


#endif /* __INC_VM_H */
