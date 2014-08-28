#ifndef __INC_SERIAL_H
#define __INC_SERIAL_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chall.h"

typedef enum {WAIT, SEND, SENT} TX_STATUS;
typedef enum {STDBY, RECV, READY} RX_STATUS;


uchar serial_read_callback(uchar data[]);
uchar serial_pending_callback(uchar data[8]);
uchar serial_write_callback(uchar data[8]);
uchar serial_check_callback(uchar data[8]);

uchar usbFunctionSetup(uchar data[8]);
uchar usbFunctionWrite(uchar *data, uchar len);
int serial_write8(char *buffer, size_t size);
int serial_write(char *buffer, size_t size);
int serial_print(char *buffer);
int serial_readln(char *line, int size);
int serial_readln_timeout(char *line, int size, int timeout);
void initForUsbConnectivity(void);
void poll();

#endif /* __INC_SERIAL_H */
