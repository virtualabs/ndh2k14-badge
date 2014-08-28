/* Name: main.c
 * Project: Ndh2k14 badge challenge
 * Author: Damien Cauquil
 * License: GNU GPL v2 (see License.txt)
 *
 * Challenge consists of four main tasks:
 *
 * - Caesar encoded string (easy one)
 * - 4PPM encoded data sent out through D0, containing the flag
 * - Externally biased PRNG used with RC4 cipher
 * - little virtual machine with obfuscated instructions and junked bytecode
 *
 *
 * 1. Caesar encoded string
 *
 * The easy one. Obvious.
 *
 *
 * 2. 4PPM encoded data sent out  through D0
 *
 * This task may be defeated in two ways:
 *
 * - reverse engineering of the firmware (hardcoded RC4 encrypted string)
 * - through a logical analyzer (Raspberry Pi + piscope)
 *
 *
 * 3. Externally biased PRNG
 *
 * This tasks ask for an authentication code based on a challenge
 * to solve: this challenge is produced by an RC4-based PRNG.
 * The PRNG relies on D1 as a floating pin, which state is volatile.
 * If this pin is grounded, then it is no more floating and PRNG values
 * may be predicted.
 *
 * This test is presented as an OTP feature for a secure access. The key
 * is unciphered in memory, used to initialize a crypto context, and then
 * ciphered.
 *
 *
 * 4. Little obfuscated VM
 *
 * Access code is processed through a double checksum check, then used as a
 * decryption key to display the final URL.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include "chall.h"
#include "rc4.h"
#include "serial.h"
#include "stages.h"
#include "hash.h"
#include "resources.h"
#include "vm/vm.h"

/******************************
 * Challenges stuff
 *****************************/

/**
 * Welcome
 */

void welcome(void)
{
    char code[9];
    HASH h;

    serial_print("Access code: ");
    
    memset(code, 0, 9);
    serial_readln(code, 9);
    hash(&h, code, 8);

    if (hashcmp(&h, (unsigned char *)STAGE1_HASH)) {
        stage1(code);
    }
    else if (hashcmp(&h, (unsigned char *)STAGE2_HASH)) {
        stage2(code);
    } 
    else if (hashcmp(&h, (unsigned char *)STAGE3_HASH)) {
        stage3(code);
    }
    else if (hashcmp(&h, (unsigned char *)STAGE4_HASH)) {
        stage4(code);
    }
}




/*****************************
 * Main code 
 ****************************/

int main (void)
{
    _delay_ms(10);
    
    /* Button setup */
    DDRB |= _BV(5);
    PINB = _BV(3);

    /* USB Setup */
    initForUsbConnectivity();


    /* Main loop */
    serial_print("Welcome to Ndh2k14 Secure Chip.\n\n");
    serial_print("New here ? type access code 'HelpPlz!'.\n\n");
    
    while(1)
    {
        welcome();
        poll();
    }

    return 0;
}

