#include "vm.h"
#include "../serial.h"
#include "syscalls.h"

#define VM_FLAG_SET(v, f) (v->flags |= f)
#define VM_FLAG_UNSET(v, f) (v->flags &= ~(f))

/**
 * VM stack operations
 */

unsigned int vm_pop(PVMContext pVMContext)
{
    if (pVMContext->sti >= 0)
        return pVMContext->stack[pVMContext->sti--];
    else
    {
        pVMContext->flags |= VM_FLAG_OF;
        return 0;
    }
}

void vm_push(PVMContext pVMContext, unsigned int value)
{
    if (pVMContext->sti >= 256)
    {
        pVMContext->flags |= VM_FLAG_OF;
        return;
    }
    else
        pVMContext->stack[++pVMContext->sti] = value;
}

void vm_push_imm(PVMContext pVMContext)
{
    unsigned int v;

    v = *((unsigned int *)(pVMContext->pc));
    pVMContext->pc += sizeof(unsigned int);
    vm_push(pVMContext, v);
}

void vm_push_off(PVMContext pVMContext)
{
    unsigned int v;

    v = *((unsigned int *)(pVMContext->pc));
    pVMContext->pc += sizeof(unsigned int);
    vm_push(pVMContext, (unsigned int)(v + ((unsigned int)pVMContext->bytecode)));
}

void vm_loadw(PVMContext pVMContext)
{
    unsigned int v,*w;

    v = vm_pop(pVMContext);
    w = (unsigned int *)v;
    vm_push(pVMContext, *w);
}

void vm_pop_off(PVMContext pVMContext)
{
    unsigned int v,*w;

    v = (*(unsigned int *)(pVMContext->pc));
    pVMContext->pc += sizeof(unsigned int);
    w = (unsigned int *)(v + (unsigned int)pVMContext->bytecode);
    *w = vm_pop(pVMContext);
}


/***************************************************
 * VM arithmetics
 **************************************************/

/**
 * Add two items from the stack and push the result.
 */

void vm_add(PVMContext pVMContext)
{
    unsigned int a;

    /* Pop first value. */
    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    /* Push result. */
    vm_push(
        pVMContext,
        vm_pop(pVMContext) + a
    );
}

/**
 * Sub two items from the stack and push the result.
 */

void vm_sub(PVMContext pVMContext)
{
    unsigned int a;

    /* Pop first value. */
    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    /* Push result. */
    vm_push(
        pVMContext,
        vm_pop(pVMContext) - a
    );
}

/**
 * Div two items from the stack and push the result.
 */

void vm_div(PVMContext pVMContext)
{
    unsigned int a;

    /* Pop first value. */
    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    /* Push result. */
    vm_push(
        pVMContext,
        vm_pop(pVMContext)/a
    );
}

/**
 * Mul two items from the stack and push the result.
 */

void vm_mul(PVMContext pVMContext)
{
    unsigned int a;

    /* Pop first value. */
    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    /* Push result. */
    vm_push(
        pVMContext,
        vm_pop(pVMContext) * a
    );
}


/***************************************************
 * VM arithmetics
 **************************************************/

/**
 * And two items from the stack and push the result.
 */

void vm_and(PVMContext pVMContext)
{
    unsigned int a;

    /* Pop first value. */
    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    /* Push result. */
    vm_push(
        pVMContext,
        vm_pop(pVMContext) & a
    );
}

/**
 * Or two items from the stack and push the result.
 */

void vm_or(PVMContext pVMContext)
{
    unsigned int a;

    /* Pop first value. */
    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    /* Push result. */
    vm_push(
        pVMContext,
        vm_pop(pVMContext) | a
    );
}

/**
 * Xor two items from the stack and push the result.
 */

void vm_xor(PVMContext pVMContext)
{
    unsigned int a;

    /* Pop first value. */
    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    /* Push result. */
    vm_push(
        pVMContext,
        vm_pop(pVMContext) ^ a
    );
}

/**
 * Not first item from the stack and push the result.
 */

void vm_not(PVMContext pVMContext)
{
    /* Pop first value. */
    vm_push(
        pVMContext,
        ~vm_pop(pVMContext)
    );
}


/***************************************************
 * VM comparison
 **************************************************/

/**
 * Compare two values.
 */

void vm_cmp(PVMContext pVMContext)
{
    unsigned int a,b;

    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    b = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    pVMContext->flags = 0;
    if (a == b) {
        VM_FLAG_SET(pVMContext, VM_FLAG_ZF);
    }
    else if (a < b) {
        VM_FLAG_SET(pVMContext, VM_FLAG_BF);
    } else if (a > b) {
        VM_FLAG_SET(pVMContext, VM_FLAG_AF);
    }
}


/**
 * Test two values.
 */

void vm_test(PVMContext pVMContext)
{
    unsigned int a, b;

    a = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    b = vm_pop(pVMContext);
    if (ERROR(pVMContext))
        return;

    if ((a ^ b) == 0) {
        VM_FLAG_SET(pVMContext, VM_FLAG_ZF);
    }
    else {
        VM_FLAG_UNSET(pVMContext, VM_FLAG_ZF);
    }
}


/***************************************************
 * VM control flow
 **************************************************/

void vm_getpc(PVMContext pVMContext)
{
    vm_push(pVMContext, (unsigned int)pVMContext->pc);
}

void vm_jmp(PVMContext pVMContext)
{
    unsigned int delta;

    /* Read 4 bytes from PC. */
    delta = *(unsigned int *)(pVMContext->pc);
    pVMContext->pc += sizeof(unsigned int);

    /* Compute new address. */
    pVMContext->pc += delta;
}

void vm_jz(PVMContext pVMContext)
{
    unsigned int delta;

    /* Read 4 bytes from PC. */
    delta = *(unsigned int *)(pVMContext->pc);
    pVMContext->pc += sizeof(unsigned int);

    /* Compute new address. */
    if (IS_ZERO(pVMContext))
        pVMContext->pc += delta;
}

void vm_jnz(PVMContext pVMContext)
{
    unsigned int delta;

    /* Read 4 bytes from PC. */
    delta = *(unsigned int *)(pVMContext->pc);
    pVMContext->pc += sizeof(unsigned int);

    /* Compute new address. */
    if (!IS_ZERO(pVMContext))
        pVMContext->pc += delta;
}

void vm_ja(PVMContext pVMContext)
{
    unsigned int delta;

    /* Read 4 bytes from PC. */
    delta = *(unsigned int *)(pVMContext->pc);
    pVMContext->pc += sizeof(unsigned int);

    /* Compute new address. */
    if (IS_ABOVE(pVMContext))
        pVMContext->pc += delta;
}

void vm_jb(PVMContext pVMContext)
{
    unsigned int delta;

    /* Read 4 bytes from PC. */
    delta = *(unsigned int *)(pVMContext->pc);
    pVMContext->pc += sizeof(unsigned int);

    /* Compute new address. */
    if (IS_BELOW(pVMContext))
        pVMContext->pc += delta;
}

void vm_syscall(PVMContext pVMContext)
{
    unsigned int syscall;

    syscall = vm_pop(pVMContext);
    switch(syscall)
    {
        case VM_SYS_READ:
            vm_sys_read(pVMContext);
            break;

        case VM_SYS_WRITE:
            vm_sys_write(pVMContext);
            break;

        case VM_SYS_LED:
            vm_sys_led(pVMContext);
            break;
    }
}

/**
 * VM init
 */

void vm_init(PVMContext pVMContext, unsigned char *bytecode)
{
    /* VM context initialization. */
    pVMContext->pc = bytecode;
    pVMContext->bytecode = bytecode;
    pVMContext->sti = -1;
    pVMContext->flags = 0;
}

/**
 * VM Main code: interpreter
 */

unsigned int vm_run(PVMContext pVMContext)
{
    char op;

    /* Loop until RET reached. */
    while (((op = *(pVMContext->pc++)) != VM_INST_RET) && (!ERROR(pVMContext)))
    {
        /* Read first byte. */
        switch(op)
        {
            case VM_INST_ADD:
                vm_add(pVMContext);
                break;

            case VM_INST_SUB:
                vm_sub(pVMContext);
                break;

            case VM_INST_DIV:
                vm_div(pVMContext);
                break;

            case VM_INST_MUL:
                vm_mul(pVMContext);
                break;

            case VM_INST_AND:
                vm_and(pVMContext);
                break;

            case VM_INST_OR:
                vm_or(pVMContext);
                break;

            case VM_INST_XOR:
                vm_xor(pVMContext);
                break;

            case VM_INST_NOT:
                vm_not(pVMContext);
                break;

            case VM_INST_CMP:
                vm_cmp(pVMContext);
                break;

            case VM_INST_TEST:
                vm_test(pVMContext);
                break;

            case VM_INST_GETPC:
                vm_getpc(pVMContext);
                break;

            case VM_INST_JMP:
                vm_jmp(pVMContext);
                break;

            case VM_INST_JNZ:
                vm_jnz(pVMContext);
                break;

            case VM_INST_JZ:
                vm_jz(pVMContext);
                break;

            case VM_INST_JA:
                vm_ja(pVMContext);
                break;

            case VM_INST_JB:
                vm_jb(pVMContext);
                break;
            case VM_INST_SYSCALL:
                vm_syscall(pVMContext);
                break;

            case VM_INST_PUSH:
                vm_push_imm(pVMContext);
                break;

            case VM_INST_PSHOFF:
                vm_push_off(pVMContext);
                break;

            case VM_INST_LOADW:
                vm_loadw(pVMContext);
                break;

            case VM_INST_POPOFF:
                vm_pop_off(pVMContext);
                break;

            case VM_INST_POP:
                vm_pop(pVMContext);
                break;

            default:
                break;
        }
    }
    return vm_pop(pVMContext);
}
