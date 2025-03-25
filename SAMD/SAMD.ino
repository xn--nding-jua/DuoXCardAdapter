/*
  DuoXCardAdapter - Connect two Behringer X-Cards with each other or use one Card as a audio-source
  (c) 2025 Chris Noeding
  v1.0.0 built on 25.03.2025
  https://github.com/xn--nding-jua/DuoXCardAdapter
  

  Pin information:
  =====================================================================
  +------------+------------------+--------+--------+-----------------+--------+-----------------------+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  | Pin number |  MKR  Board pin  |  PIN   |  FPGA  | Notes           | Peri.A |     Peripheral B      | Perip.C | Perip.D | Peri.E | Peri.F | Periph.G | Periph.H | Usage in this project                                     |
  |            |                  |        |        |                 |   EIC  | ADC |  AC | PTC | DAC | SERCOMx | SERCOMx |  TCCx  |  TCCx  |    COM   | AC/GLCK  |                                                           |
  |            |                  |        |        |                 |(EXTINT)|(AIN)|(AIN)|     |     | (x/PAD) | (x/PAD) | (x/WO) | (x/WO) |          |          |                                                           |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  | 00         | D0               |  PA22  |  G1    |                 |  *06   |     |     | X10 |     |   3/00  |   5/00  |* TC4/0 | TCC0/4 |          | GCLK_IO6 | -> Uart TxD to X-LIVE #2                                  |
  | 01         | D1               |  PA23  |  N3    |                 |  *07   |     |     | X11 |     |   3/01  |   5/01  |* TC4/1 | TCC0/5 | USB/SOF  | GCLK_IO7 | <- Uart RxD from X-LIVE #2                                |
  | 02         | D2               |  PA10  |  P3    |                 |   10   | *18 |     | X02 |     |   0/02  |   2/02  |*TCC1/0 | TCC0/2 | I2S/SCK0 | GCLK_IO4 | FrameSync out to X-LIVE #1 and #2                         |
  | 03         | D3               |  PA11  |  R3    |                 |   11   | *19 |     | X03 |     |   0/03  |   2/03  |*TCC1/1 | TCC0/3 | I2S/FS0  | GCLK_IO5 | BitClock out to X-LIVE #1 and #2                          |
  | 04         | D4               |  PB10  |  T3    |                 |  *10   |     |     |     |     |         |   4/02  |* TC5/0 | TCC0/4 | I2S/MCK1 | GCLK_IO4 | TDM0 in from X-LIVE #1                                    |
  | 05         | D5               |  PB11  |  T2    |                 |  *11   |     |     |     |     |         |   4/03  |* TC5/1 | TCC0/5 | I2S/SCK1 | GCLK_IO5 | TDM1 in from X-LIVE #1                                    |
  | 06         | D6               |  PA20  |  G16   |                 |  *04   |     |     | X08 |     |   5/02  |   3/02  |        |*TCC0/6 | I2S/SCK0 | GCLK_IO4 | TDM2 in from X-LIVE #1                                    |
  | 07         | D7               |  PA21  |  G15   |                 |  *05   |     |     | X09 |     |   5/03  |   3/03  |        |*TCC0/7 | I2S/FS0  | GCLK_IO5 | TDM3 in from X-LIVE #1                                    |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  |            |       SPI        |        |        |                 |        |     |     |     |     |         |         |        |        |          |          |                                                           |
  | 08         | MOSI             |  PA16  |  F16   |                 |  *00   |     |     | X04 |     |  *1/00  |   3/00  |*TCC2/0 | TCC0/6 |          | GCLK_IO2 | TDM0 out to X-LIVE #1                                     |
  | 09         | SCK              |  PA17  |  F15   |                 |  *01   |     |     | X05 |     |  *1/01  |   3/01  | TCC2/1 | TCC0/7 |          | GCLK_IO3 | TDM1 out to X-LIVE #1                                     |
  | 10         | MISO             |  PA19  |  C16   |                 |   03   |     |     | X07 |     |  *1/03  |   3/03  |* TC3/1 | TCC0/3 | I2S/SD0  | AC/CMP1  | TDM2 out to X-LIVE #1                                     |
  +------------+------------------+--------+--------+-----------------+--------------------+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  |            |       Wire       |        |        |                 |        |     |     |     |     |         |         |        |        |          |          |                                                           | 
  | 11         | SDA              |  PA08  |  C15   |                 |   NMI  | *16 |     | X00 |     |  *0/00  |   2/00  | TCC0/0 | TCC1/2 | I2S/SD1  |          | X-CARD 1 Ready                                            |
  | 12         | SCL              |  PA09  |  B16   |                 |   09   | *17 |     | X01 |     |  *0/01  |   2/01  | TCC0/1 | TCC1/3 | I2S/MCK0 |          | free                                                      |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  |            |      Serial1     |        |        |                 |        |     |     |     |     |         |         |        |        |          |          |                                                           |
  | 13         | RX               |  PB23  |  C11   |                 |   07   |     |     |     |     |         |  *5/03  |        |        |          | GCLK_IO1 | <- Uart RxD from X-LIVE                                   |
  | 14         | TX               |  PB22  |  A13   |                 |   06   |     |     |     |     |         |  *5/02  |        |        |          | GCLK_IO0 | -> Uart TxD to X-LIVE                                     |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  | 15         | A0 / DAC0        |  PA02  |  C2    |                 |   02   | *00 |     | Y00 | OUT |         |         |        |        |          |          | Analog Audio Output via PDM                               |
  | 16         | A1               |  PB02  |  C3    |                 |  *02   | *10 |     | Y08 |     |         |   5/00  |        |        |          |          | Analog Audio Output via PDM                               |
  | 17         | A2               |  PB03  |  C6    |                 |  *03   | *11 |     | Y09 |     |         |   5/01  |        |        |          |          | Analog Audio Output via PDM                               |
  | 18         | A3               |  PA04  |  D1    |                 |   04   | *04 |  00 | Y02 |     |         |   0/00  |*TCC0/0 |        |          |          | Analog Audio Output via PDM                               |
  | 19         | A4               |  PA05  |  D3    |                 |   05   | *05 |  01 | Y03 |     |         |   0/01  |*TCC0/1 |        |          |          | Analog Audio Output via PDM                               |
  | 20         | A5               |  PA06  |  F3    |                 |   06   | *06 |  02 | Y04 |     |         |   0/02  | TCC1/0 |        |          |          | Analog Audio Output via PDM                               |
  | 21         | A6               |  PA07  |  G2    |                 |   07   | *07 |  03 | Y05 |     |         |   0/03  | TCC1/1 |        | I2S/SD0  |          | Analog Audio Output via PDM                               |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  |            |       USB        |        |        |                 |        |     |     |     |     |         |         |        |        |          |          |                                                           |
  | 22         |                  |  PA24  |        | USB N           |   12   |     |     |     |     |   3/02  |   5/02  |  TC5/0 | TCC1/2 | USB/DM   |          | -> Serial Rx/Tx via USB                                   |
  | 23         |                  |  PA25  |        | USB P           |   13   |     |     |     |     |   3/03  |   5/03  |  TC5/1 | TCC1/3 | USB/DP   |          | -> Serial Rx/Tx via USB                                   |
  | 24         |                  |  PA18  |        | USB ID          |   02   |     |     | X06 |     |   1/02  |   3/02  |  TC3/0 | TCC0/2 |          | AC/CMP0  |                                                           |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  | 25         | AREF             |  PA03  |  B1    |                 |   03   |  01 |     | Y01 |     |         |         |        |        |          |          | TDM3 out to X-LIVE #1                                     |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  |            | FPGA             |        |        |                 |        |     |     |     |     |         |         |        |        |          |          |                                                           |
  | 26         |                  |  PA12  |  H4    | FPGA TDI        |   12   |     |     |     |     |  *2/00  |   4/00  | TCC2/0 | TCC0/6 |          | AC/CMP0  | -> [JTAG]                                                 |
  | 27         |                  |  PA13  |  H3    | FPGA TCK        |   13   |     |     |     |     |  *2/01  |   4/01  | TCC2/1 | TCC0/7 |          | AC/CMP1  | -> [JTAG]                                                 |
  | 28         |                  |  PA14  |  J5    | FPGA TMS        |   14   |     |     |     |     |   2/02  |   4/02  |  TC3/0 | TCC0/4 |          | GCLK_IO0 | -> [JTAG]                                                 |
  | 29         |                  |  PA15  |  J4    | FPGA TDO        |   15   |     |     |     |     |  *2/03  |   4/03  |  TC3/1 | TCC0/5 |          | GCLK_IO1 | -> [JTAG]                                                 |
  | 30         |                  |  PA27  |  E2    | FPGA CLK        |   15   |     |     |     |     |         |         |        |        |          | GCLK_IO0 | -> 48 MHz Clock -> FPGA                                   |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  | 31         |                  |  PA28  |  L16   | FPGA MB INT     |   08   |     |     |     |     |         |         |        |        |          | GCLK_IO0 | -> NINA RESET_N                                           |
  | 32         |                  |  PB08  |        | LED_RED_BUILTIN |   08   |  02 |     | Y14 |     |         |   4/00  |  TC4/0 |        |          |          | -> LED                                                    |
  | 33         |                  |  PB09  |        | SAM_INT_OUT     |  *09   |  03 |     | Y15 |     |         |   4/01  |  TC4/1 |        |          |          |                                                           |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
  |            | 32768Hz Crystal  |        |        |                 |        |     |     |     |     |         |         |        |        |          |          |                                                           |
  | 34         |                  |  PA00  |        | XIN32           |   00   |     |     |     |     |         |   1/00  | TCC2/0 |        |          |          |                                                           |
  | 35         |                  |  PA01  |        | XOUT32          |   01   |     |     |     |     |         |   1/01  | TCC2/1 |        |          |          |                                                           |
  +------------+------------------+--------+--------+-----------------+--------+-----+-----+-----+-----+---------+---------+--------+--------+----------+----------+-----------------------------------------------------------+
*/

#include "SAMD.h"

void ticker100msFcn() {
  ledCounter--;
  if (ledCounter == 0) {
    ledCounter = 10;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
Ticker ticker100ms(ticker100msFcn, 100, 0, MILLIS);

void setup() {
  // initialize peripherals
  pinMode(LED_BUILTIN, OUTPUT); // red color on RGB-LED of Vidor4000
  pinMode(CARD_READY_PIN, INPUT); // CardReady-signal #1 is on SDA

  // initialize communication
  // Serial for communication via USB
  Serial.begin(115200);
  Serial.setTimeout(1000); // Timeout for commands

  // initialize FPGA
  setup_fpga();
  delay(1000); // give FPGA a second to startup

  // Serial for communication with Behringer X32 MainControl
  Serial.println("Dual X-CARD Controller " + String(versionstring) + " | " + String(compile_date)); // send to USB
  Serial.println(F("Init UART-ports..."));
  SerialCard1.begin(38400); // X32 uses regular 38400 8N1 for communication
  SerialCard1.setTimeout(500); // Timeout for commands
  SerialCard2.begin(38400);
  SerialCard2.setTimeout(500); // Timeout for commands
  pinPeripheral(0, PIO_SERCOM); //Assign TX function to pin 0
  pinPeripheral(1, PIO_SERCOM); //Assign RX function to pin 1


  // initialize the X32-Cards
  Serial.println(F("Init X32-Cards..."));
  x32InitCard1();
  //x32InitCard2(); // card#2 can be initialized via USB using ASCII-commands

  // start ticker
  Serial.println(F("Init timers..."));
  ticker100ms.start();

  Serial.println(F("Ready."));
}

void loop() {
  handleUSBCommunication(); // communication through Serial (USB) with connected computer
  handleXCard1Communication();
  handleXCard2Communication();

  ticker100ms.update();
}
