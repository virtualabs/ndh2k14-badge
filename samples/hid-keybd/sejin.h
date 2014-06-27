#ifndef SEJIN_H
#define SEJIN_H

#define MOD_LCTRL   0x01
#define MOD_LSHIFT  0x02
#define MOD_LALT    0x04
#define MOD_LGUI    0x08
#define MOD_RCTRL   0x10
#define MOD_RSHIFT  0x20
#define MOD_RALT    0x40
#define MOD_RGUI    0x80

#define NUM_LOCK 1
#define CAPS_LOCK 2
#define SCROLL_LOCK 4

/**
 * USB Interface stuff
 */

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

typedef struct
{
    uint8_t modifier; // bit flags for shift, ctrl, and alt, and other stuff
    uint8_t reserved; // useless for now
    uint8_t key[6];   // HID keycodes
} keyboard_report; 
static keyboard_report reportBuffer;


/**
 * Infrared protocol packet
 */

typedef struct tPkt {
    unsigned char cDevice;
    unsigned char cModifiers;
    unsigned char cKeyCode;
    unsigned char cChecksum;
} IRPKT, *PIRPKT; 

/**
 * Decoder state machine
 */

static enum {
    STATE_WAIT,
    STATE_SEND,
    STATE_RELEASE
} recv_state;


/**
 * Sejin SWK-8650WS Keymap
 */

#define MAX_KEYMAP  0x7D

unsigned char gbKeyMap[] = {
  0xFF,  /* 00: UNKNOWN */
  0xFF,  /* 01: UNKNOWN */
  0x05,  /* 02: B */
  0x2B,  /* 03: TAB */
  0x4F,  /* 04: RIGHT */
  0x2C,  /* 05: SPACEBAR */
  0x36,  /* 06: COMMA */
  0xE1,  /* 07: LEFT SHIFT */
  0x2D,  /* 08: MINUS, UNDERSCORE */
  0x2F,  /* 09: {[ */
  0xFF,  /* 0A: UNKNOWN */
  0x28,  /* 0B: ENTER */
  0x49,  /* 0C: INS/NUM (FN) */
  0x50,  /* 0D: LEFT */
  0xFF,  /* 0E: UNKNOWN */
  0xFF,  /* 0F: UNKNOWN */
  0xFF,  /* 10: UNKNOWN */
  0xFF,  /* 11: UNKNOWN */
  0x04,  /* 12: A */
  0x16,  /* 13: S */
  0x07,  /* 14: D */
  0x09,  /* 15: F */
  0x0D,  /* 16: J */
  0xFF,  /* 17: UNKNWON */
  0x0E,  /* 18: K */
  0x0F,  /* 19: L */
  0xFF,  /* 1A: UNKNOWN */
  0x33,  /* 1B: :; */
  0x4E,  /* 1C: PAGEDWN */
  0x4D,  /* 1D: END */
  0xE3,  /* 1E: LEFT GUI KEY */  
  0xFF,  /* 1F: UNKNOWN */
  0xFF,  /* 20: UNKNWON */
  0xE6,  /* 21: RIGHT ALT */
  0x14,  /* 22: Q */
  0x1A,  /* 23: W */
  0x08,  /* 24: E */
  0x15,  /* 25: R */
  0x18,  /* 26: U */
  0xFF,  /* 27: UNKNOWN */
  0x0C,  /* 28: I */
  0x12,  /* 29: O */
  0xFF,  /* 2A: UNKNOWN */
  0x13,  /* 2B: P */
  0x30,  /* 2C: }] */
  0x48,  /* 2D: PAUSE */
  0xFF,  /* 2E: UNKNOWN */
  0xFF,  /* 2F: UNKNOWN */
  0xFF,  /* 30: UNKNOWN */
  0xFF,  /* 31: UNKNWON */
  0x1D,  /* 32: Z */
  0x1B,  /* 33: X */
  0x06,  /* 34: C */
  0x19,  /* 35: V */
  0x10,  /* 36: M */
  0xE5,  /* 37: RIGHT SHIFT */
  0xFF,  /* 38: UNKNOWN */
  0x37,  /* 39: >. */
  0xFF,  /* 3A: UNKNOWN */
  0x38,  /* 3B: ?/ */
  0x31,  /* 3C: \ */
  0x11,  /* 3D: N */
  0xFF,  /* 3E: UNKNOWN */
  0xFF,  /* 3F: 2 (alpha) */
  0xFF,  /* 40: UNKNOWN */
  0x4C,  /* 41: DELETE */
  0x3C,  /* 42: F3 */
  0x29,  /* 43: ESC */
  0x3E,  /* 44: F5 */
  0x22,  /* 45: 5 (alpha) */
  0x23,  /* 46: 6 (alpha) */
  0xFF,  /* 47: UNKNOWN */
  0x3F,  /* 48: F6 */
  0x41,  /* 49: F8 */
  0xFF,  /* 4A: UNKNOWN */
  0x2E,  /* 4B: =+ */
  0x2A,  /* 4C: BACKSPACE */
  0x4A,  /* 4D: HOME */
  0x20,  /* 4E: #~ */
  0xFF,  /* 4F: UNKNOWN */
  0xFF,  /* 50: UNKNOWN */
  0xFF,  /* 51: UNKNOWN */
  0x3B,  /* 52: F2 */
  0x3A,  /* 53: F1 */
  0x3D,  /* 54: F4 */
  0x0A,  /* 55: G */
  0x0B,  /* 56: H */
  0xFF,  /* 57: UNKNOWN */
  0x40,  /* 58: F7 */
  0x42,  /* 59: F9 */
  0xFF,  /* 5A: UNKNOWN */
  0xFF,  /* 5B: UNKNOWN */
  0x52,  /* 5C: UP */
  0x4B,  /* 5D: PAGE UP */
  0xFF,  /* 5E: UNKNOWN */
  0xE0,  /* 5F: LEFT CTRL */
  0xFF,  /* 60: FN - not supported now */
  0xFF,  /* 61: UNKNOWN */
  0x1E,  /* 62: 1 (aplpha) */
  0x1F,  /* 63: 2 (alpha) */
  0x20,  /* 64: 3 (alpha) */
  0x21,  /* 65: 4 (alpja) */
  0x24,  /* 66: 7 (alpha) */
  0xFF,  /* 67: UNKNOWN */
  0x25,  /* 68: 8 (alpha) */
  0x26,  /* 69: 9 (alpha) */
  0xFF,  /* 6A: UNKNOWN */
  0x27,  /* 6B: 0 (alpha) */
  0x51,  /* 6C: DOWN */
  0xE7,  /* 6D: WIN RIGHT GUI */
  0xFF,  /* 6E: UNKNOWN */
  0xFF,  /* 6F: UNKNOWN */
  0xFF,  /* 70: UNKNOWN */
  0xE2,  /* 71: LEFT ALT */
  0x32,  /* 72: ~ */
  0x39,  /* 73: CAPS LOCK */
  0xFF,  /* 74: UNKNOWN */
  0x17,  /* 75: T */
  0x1C,  /* 76: Y */
  0xFF,  /* 77: UNKNOWN */
  0x44,  /* 78: F11 */
  0x43,  /* 79: F10 */
  0xFF,  /* 7A: UNKNOWN */
  0x45,  /* 7B: F12 */
  0x46,  /* 7C: PRINT SCR */
};
#endif /* SEJIN_H */

