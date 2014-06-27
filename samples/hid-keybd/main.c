#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <string.h>
#include "usbdrv.h"


/**
 * Globals
 */

#define MOD_LCTRL   0x01
#define MOD_LSHIFT  0x02
#define MOD_LALT    0x04
#define MOD_LGUI    0x08
#define MOD_RCTRL   0x10
#define MOD_RSHIFT  0x20
#define MOD_RALT    0x40
#define MOD_RGUI    0x80

#define KEY_SEP     0x00
#define KEY_A       0x14
#define KEY_B       0x05
#define KEY_C       0x06
#define KEY_D       0x07
#define KEY_E       0x08
#define KEY_F       0x09
#define KEY_G       0x0A
#define KEY_H       0x0B
#define KEY_I       0x0C
#define KEY_J       0x0D
#define KEY_K       0x0E
#define KEY_L       0x0F
#define KEY_M       0x10
#define KEY_N       0x11
#define KEY_O       0x12
#define KEY_P       0x13
#define KEY_Q       0x14
#define KEY_R       0x15
#define KEY_S       0x16
#define KEY_T       0x17
#define KEY_U       0x18
#define KEY_V       0x19
#define KEY_W       0x1D
#define KEY_X       0x1B
#define KEY_Y       0x1C
#define KEY_Z       0x1A
#define KEY_SP      0x2C
#define KEY_ENTER   0x28

static uchar    idleRate;   /* repeat rate for keyboards, never used for mice */

typedef struct
{
    uint8_t modifier; // bit flags for shift, ctrl, and alt, and other stuff
    uint8_t reserved; // useless for now
    uint8_t key[6];   // HID keycodes
} keyboard_report; 
static keyboard_report reportBuffer;

/**************************************
 * Char sequence to send to the host
 *************************************/

typedef struct {
    unsigned char mod;
    unsigned char key;
} KEY,*PKEY;
    
static KEY gSequence[] = {
    {0x00, KEY_E}, /* e */
    {0x00, KEY_SEP},
    {0x00, KEY_C}, /* c */
    {0x00, KEY_SEP},
    {0x00, KEY_H}, /* h */
    {0x00, KEY_SEP},
    {0x00, KEY_O}, /* o */
    {0x00, KEY_SEP},
    {0x00, KEY_SP}, /*   */
    {0x00, KEY_SEP},
    {0x20, KEY_H}, /* H */
    {0x00, KEY_SEP},
    {0x00, KEY_E}, /* e */
    {0x00, KEY_SEP},
    {0x00, KEY_L}, /* l */
    {0x00, KEY_SEP},
    {0x00, KEY_L}, /* l */
    {0x00, KEY_SEP},
    {0x00, KEY_O}, /* o */
    {0x00, KEY_SEP},
    {0x00, KEY_SP}, /*   */
    {0x00, KEY_SEP},
    {0x00, KEY_W}, /* w */
    {0x00, KEY_SEP},
    {0x00, KEY_O}, /* o */
    {0x00, KEY_SEP},
    {0x00, KEY_R}, /* r */
    {0x00, KEY_SEP},
    {0x00, KEY_L}, /* l */
    {0x00, KEY_SEP},
    {0x00, KEY_D}, /* d */
    {0x00, KEY_SEP},
    {0x00, KEY_ENTER}, /* <- */
    {0x00, KEY_SEP},
    {0xFF, 0xFF}
};

static int seq_pos = 0;
static int seq_done = 0;
static int seq_sending = 0;

/**************************************************************************
 * USB MANAGEMENT
 *************************************************************************/

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)(224)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs) ; Modifier byte
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs) ; Reserved byte
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs) ; LED report
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs) ; LED report padding
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))(0)
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)(101)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

/**
 * USB Device Setup callback
 */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;

    /* The following requests are never used. But since they are required by
     * the specification, we implement them in this example.
     */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){  /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* we only have one report type, so don't look at wValue */
            usbMsgPtr = (void *)&reportBuffer;
            reportBuffer.modifier = 0;
            reportBuffer.key[0] = 0;
            return sizeof(reportBuffer);
        } else if (rq->bRequest == USBRQ_HID_SET_REPORT) {
            return (rq->wLength.word == 1) ? USB_NO_MSG : 0;
        }else if(rq->bRequest == USBRQ_HID_GET_IDLE){
            usbMsgPtr = &idleRate;
            return 1;
        }else if(rq->bRequest == USBRQ_HID_SET_IDLE){
            idleRate = rq->wValue.bytes[1];
        }
    }else{
        /* no vendor specific requests implemented */
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}


/**
 * Main code
 */

int main (void)
{
    unsigned char keystroke = 0;


    /* USB Setup */
    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    _delay_ms(250);
    usbDeviceConnect();
    sei();

    /* Reset reportBuffer */
    memset(&reportBuffer, 0, sizeof(keyboard_report));

    DDRB &= ~(_BV(DDB3));
    PORTB |= _BV(DDB3);

    /* Main loop */
    while(1)
    {
        /* Pöll USB */
        usbPoll();

        if ((PINB & _BV(3)) == 0) {
            seq_done = 0;
            seq_pos = 0;
            seq_sending = 0;
        }

        /* If we can write something to USB, check if something is pending */
        if (usbInterruptIsReady() && (seq_sending > 0)) {
            if (seq_sending == 2) {
                /* We have sent the keypress and waited for a second usbPoll(). Ok. */
                seq_sending = 0;
            } else {
                /* Set the interrupt, and wait for a second usbPoll(). */
                usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
                seq_sending = 2;
            }
        }

        /* Send from sequence */
        if (!seq_done && (seq_sending == 0)) {
            /* Is it the end of the sequence ? */
            if ((gSequence[seq_pos].mod != 0xFF) && (gSequence[seq_pos].key != 0xFF)) {
                /* Preparing report ... */
                reportBuffer.modifier = gSequence[seq_pos].mod;
                reportBuffer.key[0] = gSequence[seq_pos].key;
                seq_sending = 1;
                /* ... and going one step further. */
                seq_pos++;
            } else {
                /* If end is reached, send a null report. */
                memset(&reportBuffer, 0, sizeof(reportBuffer));
                seq_sending = 1;

                /* Mark sequence as sent. */
                seq_done = 1;
            }
        }
    }
}

