#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "serial.h"
#include "syscalls.h"


/**
 * Read syscall
 *
 * ST[0]: destination buffer
 * ST[1]: destination size
 *
 * Returns the size read into the first stack item.
 */

void vm_sys_read(PVMContext pVMContext)
{
    char *buffer = NULL;
    int size = 0;

    buffer = (char *)vm_pop(pVMContext);
    size = (int)vm_pop(pVMContext);

    vm_push(
        pVMContext,
        serial_readln(buffer, size)
    );
}


/**
 * Write syscall
 *
 * ST[0]: Source buffer
 * ST[1]: source size
 *
 * Returns the number of bytes written in the first stack
 * item.
 */

void vm_sys_write(PVMContext pVMContext)
{
    char *buffer = NULL;
    int size = 0;

    buffer = (char *)vm_pop(pVMContext);

    vm_push(
        pVMContext,
        serial_print(buffer)
    );
}


/**
 * Led control
 *
 * ST[0]: ON if != 0, OFF otherwise
 */

void vm_sys_led(PVMContext pVMContext)
{
    unsigned int oldDDRB = DDRB;
    unsigned int status;

    DDRB |= _BV(5);

    status = vm_pop(pVMContext);
    if (status)
        PORTB |= _BV(5);
    else
        PORTB &= ~(_BV(5));

    DDRB = oldDDRB;
}
