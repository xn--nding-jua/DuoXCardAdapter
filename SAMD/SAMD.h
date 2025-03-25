const char* versionstring = "v1.0.0";
const char compile_date[] = __DATE__ " " __TIME__;

#include <wiring_private.h>
#include "jtag.h"
#include "fpga.h"
#include "Ticker.h"

#define CARD_READY_PIN       11

// includes for Serial2 to communicate with NINA. As Serial2 is not within the scope of Arduino,
// we have to create it using the SERCOM-system of the SAMD21
#define PIN_SERIAL2_TX       (0ul)                // Pin description number for PIO_SERCOM on D0
#define PIN_SERIAL2_RX       (1ul)                // Pin description number for PIO_SERCOM on D1
#define PAD_SERIAL2_TX       (UART_TX_PAD_0)      // SERCOM pad 0 TX
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_1)    // SERCOM pad 1 RX
Uart Serial2(&sercom3, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

#define SerialCard1 Serial1
#define SerialCard2 Serial2

uint8_t ledCounter = 10;