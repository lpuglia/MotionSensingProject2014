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
#include "motion.c"
#include "chprintf.h"

/*
 * Parametri configurazione motore di movimento
 */
unsigned int motor1Pin1 = 1;    // pin 2 on L293D
unsigned int motor1Pin2 = 2;    // pin 7 on L293D
unsigned int enableMotor1 = 0;  // pin 1 on L293D
unsigned int motor2Pin1 = 7;    // pin 10 on L293D
unsigned int motor2Pin2 = 8;    // pin 15 on L293D
unsigned int enableMotor2 = 6;  // pin 9 on L293D




#define MAX_LENGTH  1400 //lunghezza max messaggi trasmessi dal modulo WiFi, si potrebbe ridurre
uint8_t buff[MAX_LENGTH]; //buffer di lettura



/*
 * Application entry point.
 */
int main(void) {

  int charbuf, cnt=0;

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
   * strutture per il movimento (i motori)
   */
  DCMotor motor1 = {
        {GPIOB, motor1Pin1},     // motor1 IN1 : PB1
        {GPIOB, motor1Pin2},     // motor1 IN2 : PB2
        {GPIOB, enableMotor1},   // motor1 EN  : PB0
        &PWMD3,                  // motor1 EN PWM Driver : TIM3
        2                        // motor1 EN PWM Channel : CH3
    };

    DCMotor motor2 = {
        {GPIOB, motor2Pin1},     // motor2 IN1 : PB7
        {GPIOB, motor2Pin2},     // motor2 IN2 : PB8
        {GPIOB, enableMotor2},   // motor2 EN  : PB6
        &PWMD4,                  // motor2 EN PWM Driver : TIM4
        0                        // motor2 EN PWM Channel : CH1
    };

    /* CONFIGURAZIONE DEI MOTORI!!! */
    DCMotorConfig(&motor1);
    DCMotorConfig(&motor2);


  /*
   * Activates the serial driver 1, PA9 and PA10 are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));       /* USART1 TX.       */
  palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));      /* USART1 RX.       */


  sdRead(&SD1,  buff, 1); //Alla connessione di un client viene generato un FF

  *buff = 0;

  char *ptr = buff; //i char sono unsigned per default
  EventListener el;
  flagsmask_t flags;
  chEvtRegisterMask((EventSource *)chnGetEventSource(&SD1), &el, EVENT_MASK(1));
  int vel=90;
  /*
   * Il server attende linee terminate da \r\n
   * e le rispedisce al client
   */
  while (TRUE) {

    chEvtWaitOneTimeout(EVENT_MASK(1), MS2ST(10));
    flags = chEvtGetAndClearFlags(&el);
    if (flags & CHN_INPUT_AVAILABLE)
    {

      char* stringa;
      do
      {
        charbuf = chnGetTimeout(&SD1, TIME_IMMEDIATE);
        if ( charbuf != Q_TIMEOUT )
        {
          if (charbuf == '\r')
          {
            if ((charbuf = sdGet(&SD1)) == '\n')
            {
              //Scelta dell'operazione da eseguire
              switch(buff[0])
              {
              case 'U': //UP
                              DCMotorForward(&motor1, vel);
                              DCMotorForward(&motor2, vel);
                              break;
              case 'D': //DOWN
                              DCMotorBackward(&motor1, vel);
                              DCMotorBackward(&motor2, vel);
                              break;
              case 'R': //RIGHT
                              DCMotorTurnR(&motor1,&motor2, vel);
                              break;
              case 'L': //LEFT
                              DCMotorTurnL(&motor1, &motor2, vel);
                              break;
              case 'V': //VELOCITY
                              sscanf( buff, "%[^=]=%d", stringa, &vel);
                              break;
              case 'X': //SPLITR
                              DCMotorSplitR(&motor1, &motor2, 90);
                              break;
              case 'Y': //SPLITL
                              DCMotorSplitL(&motor1, &motor2, 90);
                              break;
              default:
                              DCMotorStop(&motor1);
                              DCMotorStop(&motor2);
                              break;
              }
              
              *ptr = 0;
              chprintf((BaseSequentialStream*)&SD1, "%s\r\n", buff);
              *buff=0;
              ptr = buff;
            }
            else
            {
              *ptr++ = '\r';
              *ptr++ = charbuf;
            }
          }
          else
          {
            *ptr++ = charbuf;
          }
        }
      }
      while (charbuf != Q_TIMEOUT);
    }


  }
}
