#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>

#include "stages.h"
#include "rc4.h"
#include "resources.h"
#include "vm/vm.h"

#define PPM_HIGH    PORTB |= _BV(5);
#define PPM_LOW     PORTB &= ~(_BV(5));
#define PPM_WAIT(d) _delay_us(d)

static char adc_init_done = 0;

/**
 * Memory helper
 *
 * Used to clean memory allocated to avoid memory leak.
 */

void clean(char *memory, int size)
{
    memset(memory, 0x00, size);
}

/**
 * Stage 1: send a cesar encoded data
 *
 * Contestant has to decrypt the provided string and use the result
 * as an access code for level 2.
 */

void stage1(char *code)
{
    RC4 rc4;
    char buffer[STAGE1_WELCOME_LEN + 1];

    /* Load resource, decrypt and use it. */
    if (rsrc_find(STAGE1_WELCOME, buffer, STAGE1_WELCOME_LEN) == STAGE1_WELCOME_LEN)
    {
        rc4_init(&rc4, code, 8);
        rc4_cipher(&rc4, buffer, STAGE1_WELCOME_LEN);
        buffer[STAGE1_WELCOME_LEN] = '\0';
        serial_print(buffer);
        clean(buffer, STAGE1_WELCOME_LEN + 1);
    }
}

/**
 * Stage 3: D0 something
 *
 * When entered, this security level sends a string in 4PPM on D0 (during few seconds).
 * The string is extracted from memory and sent through helpers on pin D0.
 *
 * Contestant has to decode it, and provided the result code as an access
 * code for level 3.
 */

void send_4ppm_chip(char chip)
{
    /* delay: 5ms */
    int delay = 110; 

    /* Chip must be 0, 1, 2 or 3. */
    switch(chip)
    {
        case 0: /* 1000 */
            PPM_HIGH
            PPM_WAIT(delay);
            PPM_LOW
            PPM_WAIT(delay*3);
            break;

        case 1:
            PPM_LOW
            PPM_WAIT(delay);
            PPM_HIGH
            PPM_WAIT(delay);
            PPM_LOW
            PPM_WAIT(delay*2);
            break;

        case 2:
            PPM_LOW
            PPM_WAIT(delay*2);
            PPM_HIGH
            PPM_WAIT(delay);
            PPM_LOW
            PPM_WAIT(delay);
            break;

        case 3:
            PPM_LOW
            PPM_WAIT(delay*3);
            PPM_HIGH
            PPM_WAIT(delay);
            break;
    }
}

void send_4ppm_byte(char byte)
{
    int i,chip;

    for (i=0;i<4;i++)
    {
        chip = ((byte & (3 << (2*i))) >> (2*i));
        send_4ppm_chip(chip);
    }
}

void send_4ppm(char *buffer, int len)
{
    int i;

    /* Configure B3 as output. */
    DDRB |= _BV(5);
    PPM_LOW

    /* Send each byte. */
    for (i=0; i<len; i++)
        send_4ppm_byte(buffer[i]);

    PPM_LOW
}

void stage3(char *code)
{
    RC4 rc4;
    char buffer[STAGE3_WELCOME_LEN + 1];
    char payload[STAGE3_PAYLOAD_LEN + 1];
    int paylen,welcomelen;

    /* Load resources */
    welcomelen = rsrc_find(STAGE3_WELCOME, buffer, STAGE3_WELCOME_LEN);
    paylen = rsrc_find(STAGE3_PAYLOAD, payload, STAGE3_PAYLOAD_LEN);
    if ((welcomelen > 0) && (paylen > 0))
    {
        /* Decrypt buffers. */
        rc4_init(&rc4, code, 8);
        rc4_cipher(&rc4, buffer, STAGE3_WELCOME_LEN);
        buffer[STAGE3_WELCOME_LEN] = '\0';

        rc4_init(&rc4, code, 8);
        rc4_cipher(&rc4, payload, STAGE3_PAYLOAD_LEN);
        payload[STAGE3_PAYLOAD_LEN] = '\0';

        /* Display welcome message. */
        serial_print(buffer);
        clean(buffer, STAGE3_WELCOME_LEN + 1);

        /* Send 4PPM string. */
        send_4ppm(payload, STAGE3_PAYLOAD_LEN);
        clean(buffer, STAGE3_PAYLOAD_LEN + 1);
    }
}

/**
 * Stage 2: One Time Password
 *
 * When entered, this security level displays an hexdump associated with an IV.
 *
 * This is in fact some string RC4 encrypted with a random IV. The user has a few time
 * to type in the solution (only few seconds) before being rejected by the system.
 *
 * This step uses a weak PRNG based on a unused pin configured as floating input pin.
 * Set this pin to the ground, and this is no more random !
 *
 * Contestant has to decode it, and provided the result code as an access
 * code for level 3.
 */

/*************************
 * PRNG
 ************************/

void adc_init(void)
{
    if (adc_init_done == 0) {
        ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
        ADMUX |= (1<<REFS0);                //Voltage reference from internal
        ADCSRA |= (1<<ADEN);                //Turn on ADC
        adc_init_done = 1;
    }
}

void adc_close(void)
{
}

unsigned char read_adc(uint8_t channel){
    ADMUX &= 0xF0;                    //Clear the older channel that was read
    ADMUX |= channel;                //Defines the new ADC channel to be read
    ADCSRA |= (1<<ADSC);                //Starts a new conversion
    while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
    return (ADCL);                    //Returns the ADC value of the chosen channel
}

/**
 * Entropy gathering.
 */

unsigned char entropy_collect()
{
    unsigned char emit;

    emit = read_adc(0);
    return emit;
}


/**
 * PRNG initialization
 * 
 * Fills the structure with random bits read from pin B1.
 */

void prng_init(PPRNG prng)
{
    int i;

    for (i=0; i<8; i++)
        prng->bytes[i] = entropy_collect();
}


/**
 * Random char generation.
 *
 * Generates a random char from PRNG state.
 */

unsigned char prng_char(PPRNG prng)
{
    RC4 rc4;

    /* Encrypt prng bytes with rc4. */
    rc4_init(&rc4, prng->bytes, 8);
    rc4_cipher(&rc4, prng->bytes, 8);
    
    return prng->bytes[0];
}


/**
 * Random string generation.
 *
 * Generates a random printables string.
 */

void prng_string(PPRNG prng, char *buffer, int size)
{
    int i;
    unsigned int j;
    for (i=0; i<size; i++)
    {
        j = (prng_char(prng) % 62);
        buffer[i] = CHARSET[j];
    }
}

/**
 * Random string num generation.
 *
 * Generates a random printables string (with only nums).
 */

void prng_nums(PPRNG prng, char *buffer, int size)
{
    int i;
    unsigned int j;
    for (i=0; i<size; i++)
    {
        j = (prng_char(prng) % 10);
        buffer[i] = CHARSET[j];
    }
}


/**
 * Stage 2
 *
 * IV, key and string randomly chosen, user is asked to decrypt.
 * Two ways to solve this one:
 * 1. RE the code, find the crypto key and decrypt the payload (access code)
 * 2. Connect C0 to GND and wait for a stable biased PRNG. Offline decryption + exploit.
 *
 */

void stage2(char *code)
{
    PRNG prng;
    RC4 rc4;
    char key[9];
    unsigned char question[9];
    char answer[9];
    char question_hex[17];
    char iv_hex[9];
    char buffer[STAGE2_WELCOME_LEN + 1];
    char payload[STAGE2_PAYLOAD_LEN + 1];
    int welcomelen, paylen;

    /* Init PRNG. */
    if (adc_init_done == 0)
    {
        adc_init();
        prng_init(&prng);
    }

    /* Generate a random key. */
    prng_string(&prng, key, 8);
    key[8] = '\0';
    
    /* Generate question. */
    prng_nums(&prng, (char *)question, 8);
    question[8] = '\0';
    rc4_init(&rc4, key, 8);
    rc4_cipher(&rc4, (char *)question, 8);

    snprintf(
        question_hex,
        17,
        "%02x%02x%02x%02x%02x%02x%02x%02x",
        question[0],
        question[1],
        question[2],
        question[3],
        question[4],
        question[5],
        question[6],
        question[7]
    );

    snprintf(
        iv_hex,
        9,
        "%02x%02x%02x%02x",
        key[0],
        key[1],
        key[2],
        key[3]
    );
    
    welcomelen = rsrc_find(STAGE2_WELCOME, buffer, STAGE2_WELCOME_LEN);
    paylen = rsrc_find(STAGE2_PAYLOAD, payload, STAGE2_PAYLOAD_LEN);
    if ((welcomelen > 0) && (paylen > 0))
    {
        /* Decrypt buffers. */
        rc4_init(&rc4, code, 8);
        rc4_cipher(&rc4, buffer, STAGE2_WELCOME_LEN);
        buffer[STAGE2_WELCOME_LEN] = '\0';

        /* Displays question. */
        serial_print(buffer);
        clean(buffer, STAGE2_WELCOME_LEN + 1);
        serial_print("IV: ");
        serial_print(iv_hex);
        serial_print("\n");
        serial_print("RC4(RANDOM, IV+KEY): ");
        serial_print(question_hex);
            
        /* If answer is good, displays success. */
#if 0
        rc4_init(&rc4, key, 8);
        rc4_cipher(&rc4, question, 8);
        question[8] = '\0';
        serial_print("expect: ");
        serial_print(question);
        rc4_init(&rc4, key, 8);
        rc4_cipher(&rc4, question, 8);
#endif
        serial_print("\n\n");
        serial_print("\nANSWER: ");
        
        /* Ask for answer, with timeout of 10s. */
        if (serial_readln_timeout(answer, 8, 10000) >= 0)
        {
            /* If answer is good, displays success. */
            rc4_init(&rc4, key, 8);
            rc4_cipher(&rc4, (char *)question, 8);
            if (!strncmp(answer, (char *)question, 8)) {
                rc4_init(&rc4, code, 8);
                rc4_cipher(&rc4, payload, STAGE2_PAYLOAD_LEN);
                payload[STAGE2_PAYLOAD_LEN] = '\0';
                serial_print(payload);
                serial_print("\n\n");
                clean(payload, STAGE2_PAYLOAD_LEN + 1);
            }
        } else {
            serial_print("\nTimeout reached.\n\n");
        }
    }
}


/**
 * Stage 4
 *
 * VM stuff, see vm/vm.h.
 *
 * Contestants must reverse the vm bytecode, find out the correct passcode,
 * and then the final url will be given. Led will blink too.
 */

void stage4(char *code)
{
    char *ans;
    unsigned int v;
    char payload[] =   "\x06\x1f\x5d\x16\x35\xf5\x07\xe4\x7a\x27\xbd\xcd\xfb\x67\xf6\xab" \
                       "\x66\x3a\x45\x72\x48\xaa\xed\xab\xa5\x00\x74\xce\x29\x52\x78\x83" \
                       "\xea\xf2";
    unsigned char bytecode[]=\
    "\xe8\x6e\xc9\x96\x67\xd8\xed\xa3\x34\xa6\xec\xc6\xf9\x5d\x47\xe7\xcc\x3e\x92\xda\x31\xcb\x9c\x3d\x73\x6f\x31\xc0\x3c\xf2\x2d\xed\x36\x9b\x3d\x92\xcc\xc9\xe8\x0a\xf4\x4a\xe5\xc0\xea\x70\x49\x10\xb0\x81\x6a\xb2\x88\x97\x73\xc2\x72\xf8\x6d\x56\x09\xe2\xee\xba\xa7\xc6\xdc\xdc\x98\xeb\xa3\x9d\x31\xc3\xa5\xdc\x4a\xe3\xca\x5e\x64\x51\xf3\x55\xaf\xce\xe8\xdf\x8b\xc0\xac\xbc\xad\x01\x6d\x9b\xb9\xab\x79\xbd\xc4\xbb\x37\x80\xf3\xbb\xde\x14\xbf\xd5\xeb\xce\xaa\x77\x52\x1c\xde\x88";
    VMContext vm;
    RC4 rc4;

    /* vm test */
    rc4_init(&rc4, code, 8);
    rc4_cipher(&rc4, bytecode, 118);
    vm_init(&vm, bytecode);
    v = vm_run(&vm);
    if (v)
    {
        ans = (char *)v;
        rc4_init(&rc4, ans, 4);
        rc4_cipher(&rc4, payload, 34);
        serial_print(payload);
        serial_print("\n");
        while (1) {
            PORTB ^= _BV(5);
            _delay_ms(250);
            poll();
        }
    }
}

