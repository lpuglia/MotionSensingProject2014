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


int motor1Pin1 = 1;    // pin 2 on L293D
int motor1Pin2 = 2;    // pin 7 on L293D
int enableMotor1 = 0;  // pin 1 on L293D
int motor2Pin1 = 7;    // pin 15 on L293D
int motor2Pin2 = 8;    // pin 10 on L293D
int enableMotor2 = 6;  // pin 9 on L293D




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


// CONFIGURAZIONE DEI MOTORI
    DCMotorConfig(&motor1);
    DCMotorConfig(&motor2);

// STOP
    DCMotorStop(&motor1);
    DCMotorStop(&motor2);
    chThdSleepMilliseconds(2000);

// AVANTI
    DCMotorForward(&motor1, 60);
    DCMotorForward(&motor2, 60);
    chThdSleepMilliseconds(2000);

// FREE
    DCMotorFree(&motor1);
    DCMotorFree(&motor2);
    chThdSleepMilliseconds(1000);

// INDIETRO
    DCMotorBackward(&motor1, 50);
    DCMotorBackward(&motor2, 50);
    chThdSleepMilliseconds(2000);

// RUOTA A DESTRA
    DCMotorStop(&motor1);
    DCMotorForward(&motor2, 50);
    chThdSleepMilliseconds(1000);

// RUOTA A SINISTRA
    DCMotorForward(&motor1, 50);
    chThdSleepMilliseconds(1000);
    DCMotorStop(&motor2);

// GIRA A DESTRA
    DCMotorBackward(&motor1, 50);
    DCMotorForward(&motor2, 40);
    chThdSleepMilliseconds(2000);

// GIRA A SINISTRA
    DCMotorForward(&motor1, 45);
    DCMotorBackward(&motor2, 50);
    chThdSleepMilliseconds(2000);

// STOP
    DCMotorStop(&motor1);
    DCMotorStop(&motor2);

    return 0;
}



