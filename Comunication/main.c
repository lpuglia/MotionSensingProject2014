/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
 */

#include "ch.h"
#include "hal.h"

/*
 * Parametri configurazione Access Point
 */
static const char* AT_CHANNEL = "at+channel=6\r\n";
static const char* AT_IPSTATIC = "at+ipstatic=192.168.100.1,255.255.255.0,192.168.100.1,0,0\r\n";
static const char* AT_IPDHCP = "at+ipdhcp=1\r\n";
static const char* AT_AP = "at+ap=MotionSensing2014,0\r\n";

/*
 * Parametri configurazione TCP Server
 */
static const char* AT_LTCP = "at+ltcp=25000\r\n";

/*
 * Transparent transmission mode
 */
static const char* AT_PWRMODE = "at+pwrmode=0\r\n";
static const char* AT_EASY = "at+easy_txrx\r\n";


#define MAX_LENGTH  1400 //lunghezza max messaggi trasmessi dal modulo WiFi, si potrebbe ridurre
uint8_t buff[MAX_LENGTH]; //buffer di lettura


/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 1, PA9 and PA10 are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));       /* USART1 TX.       */
  palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));      /* USART1 RX.       */


  sdRead(&SD1,  buff, 1);
  chprintf((BaseSequentialStream *)&SD1, "%s", AT_CHANNEL);
  sdRead(&SD1,  buff, 4);
  chprintf((BaseSequentialStream *)&SD1, "%s", AT_IPSTATIC);
  sdRead(&SD1,  buff, 4);
  chprintf((BaseSequentialStream *)&SD1, "%s", AT_IPDHCP);
  sdRead(&SD1,  buff, 4);
  chprintf((BaseSequentialStream *)&SD1, "%s", AT_AP);
  sdRead(&SD1,  buff, 4);
  chprintf((BaseSequentialStream *)&SD1, "%s", AT_LTCP);
  sdRead(&SD1,  buff, 5);
  palSetPad(GPIOE, GPIOE_LED7_GREEN); //da ora e' possibile connettere un client

  /*
   * Segue una read bloccante che attende il messaggio che annuncia
   * la connessione di un client, possibile sostiture con un sistema di lettura
   * asincrono.
   */
  sdRead(&SD1,  buff, 23);

  palClearPad(GPIOE, GPIOE_LED7_GREEN);
  /*
   * Viene attivata la transparent transmission mode, in cui i
   * messaggi vengono inoltrati direttamente a server e client senza
   * invocare esplicitamente send/recv data
   */
  chprintf((BaseSequentialStream *)&SD1, "%s", AT_PWRMODE);
  sdRead(&SD1,  buff, 4);
  chprintf((BaseSequentialStream *)&SD1, "%s", AT_EASY);
  sdRead(&SD1,  buff, 4);
  chprintf((BaseSequentialStream *)&SD1, "prova di easy transmission\r\n"); //messaggio per il client TCP


  /*
   * Echo di tutti i caratteri ricevuti
   */
  while (TRUE) {
    int input = sdGet(&SD1);
    chprintf((BaseSequentialStream *)&SD1, "%c", input);
  }
}
