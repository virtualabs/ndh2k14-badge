/* Name: main.c
 * Project: Ndh2k14 badge challenge
 * Author: Damien Cauquil
 * License: GNU GPL v2 (see License.txt)
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>



/*****************************
 * Main code 
 ****************************/

int main (void)
{
    _delay_ms(10);
    
    /* Button setup */
    DDRB |= _BV(5);

    while(1)
    {
        _delay_ms(1000);
        PORTB ^= _BV(5);
    }

    return 0;
}

