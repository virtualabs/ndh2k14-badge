#include "serial.h"
#include "usbconfig.h"
#include "usbdrv/usbdrv.c"

static uchar pending[1];
static uchar outBuffer[32] = "wow very hardware much electronic";
static uchar inBuffer[32] = "Y U NO DISASSEMBLE ME ?";
static TX_STATUS sent = WAIT;
static RX_STATUS recv = STDBY;
static uchar out_size = 0;
static uchar in_size = 0, in_ptr=0, in_bytes=0;

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {    /* USB report descriptor */
0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
0x09, 0x01,                    // USAGE (Vendor Usage 1)
0xa1, 0x01,                    // COLLECTION (Application)
0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
0x75, 0x08,                    //   REPORT_SIZE (8)
0x95, 0x08,                    //   REPORT_COUNT (8)
0x09, 0x00,                    //   USAGE (Undefined)  
0x82, 0x02, 0x01,              //   INPUT (Data,Var,Abs,Buf)
0x95, 32,                      //   REPORT_COUNT (32)
0x09, 0x00,                    //   USAGE (Undefined)        
0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
0xc0                           // END_COLLECTION
};

uchar usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (usbRequest_t *)data;
    if (rq->bRequest == BADGE_SERIAL_PENDING)
    {
        return serial_pending_callback(data);
    }
    if (rq->bRequest == BADGE_SERIAL_READ)
    {
        return serial_read_callback(data);
    }
    else if (rq->bRequest == BADGE_SERIAL_WRITE)
    {
        return serial_write_callback(data);
    } else if (rq->bRequest == BADGE_SERIAL_CHECK)
    {
        return serial_check_callback(data);
    }
    return 0;
}

uchar serial_read_callback(uchar data[])
{
    /* Handles read requests. */
    if (sent == SEND) {

        /* Up to 64 bytes.*/
        usbMsgLen_t len = out_size;

        /* Tell driver where the buffer is */
        usbMsgPtr = (usbMsgPtr_t)outBuffer;
        sent = SENT;

        /* Tells driver how many bytes to send. */
        return len;
    }
    else {
        usbMsgLen_t len = 0;
        usbMsgPtr = (usbMsgPtr_t)outBuffer;
        return 0;
    }
}

uchar serial_pending_callback(uchar data[8])
{
    if (sent == SEND)
    {
        pending[0] = 'Y';
    } else {
        pending[0] = 'N';
    }

    usbMsgLen_t len = 1;
    usbMsgPtr = (usbMsgPtr_t)pending;
    return len;
}

uchar serial_write_callback(uchar data[8])
{
    usbRequest_t    *rq = (usbRequest_t *)data;

    /* Handles write requests. */
    recv = RECV;
    in_size = rq->wLength.word;
    in_bytes = in_size;
    if((in_size + in_ptr) > sizeof(inBuffer))
        in_size = (sizeof(inBuffer) - in_ptr);
    return USB_NO_MSG;
}

uchar serial_check_callback(uchar data[8])
{
    /* Copy 4 first bytes from input buffer to output buffer. */
    usbRequest_t *rq = (void *)data;

    /* Up to 64 bytes.*/
    usbMsgLen_t len = rq->wLength.word;
    /* Tell driver where the buffer is */
    usbMsgPtr = (usbMsgPtr_t)inBuffer;

    /* Tells driver how many bytes to send. */
    return len;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;
    if(len > in_size)
        len = in_size;
    in_size -= len;
    for(i = 0; i < len; i++)
        inBuffer[in_ptr++] = data[i];
    if (in_size == 0)
        recv = READY;
    return in_size == 0;
}

int serial_write8(char *buffer, size_t size)
{
    while (sent != WAIT)
        usbPoll();

    if (size > 8)
        size = 8;

    sent = SEND;
    memcpy(outBuffer, buffer, size);
    out_size = size;
    while (sent != SENT)
        usbPoll();
    while(!usbInterruptIsReady())
        usbPoll();
    sent = WAIT;
    return 1;
}

int serial_write(char *buffer, size_t size)
{
    size_t len;
    while (size > 0) {
        len = size;
        if (len >= 8) {
            len = 8;
            size -= 8;
        } else {
            size = 0;
        }
        serial_write8(buffer, len);
        buffer += len;
    }
    return 1;
}

int serial_print(char *buffer)
{
    return serial_write(buffer, strlen(buffer));
}

int serial_readln(char *line, int size)
{
    /* Wait for some data to come in. */
    while (recv != READY) {
        usbPoll();
    }
    
    /* Copy data to line. */
    if (size == 0)
        return in_bytes;
    if (size > in_bytes)
        size = in_bytes;
    memcpy(line, inBuffer, size);

    /* Flush inBuffer. */
    in_ptr = 0;
    recv = STDBY;
    return size;
}

int serial_readln_timeout(char *line, int size, int timeout)
{
    unsigned int c = 0;

    /* Wait for some data to come in. */
    while ((recv != READY) && (c<timeout)) {
        c++;
        wdt_reset();
        usbPoll();
        _delay_us(500);
    }

    if (c >= timeout)
        return -1;
    
    /* Copy data to line. */
    if (size == 0)
        return in_bytes;
    if (size > in_bytes)
        size = in_bytes;
    memcpy(line, inBuffer, size);

    /* Flush inBuffer. */
    recv = STDBY;
    in_ptr = 0;
    return size;
}

void initForUsbConnectivity(void)
{
    uchar   i = 0;

    usbInit();
    /* enforce USB re-enumerate: */
    usbDeviceDisconnect();  /* do this while interrupts are disabled */
    do{             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }while(--i);
    usbDeviceConnect();
    sei();
}

void poll() {
    usbPoll();
}

int interrupt_ready() {
    return usbInterruptIsReady();
}
