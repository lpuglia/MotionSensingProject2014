#include "ch.h"
#include "hal.h"
#include "motion.c"


unsigned int motor1Pin1 = 1;    // pin 2 on L293D
unsigned int motor1Pin2 = 2;    // pin 7 on L293D
unsigned int enableMotor1 = 0;  // pin 1 on L293D
unsigned int motor2Pin1 = 7;    // pin 10 on L293D
unsigned int motor2Pin2 = 8;    // pin 15 on L293D
unsigned int enableMotor2 = 6;  // pin 9 on L293D




int main(void) {

  /*
   * System initializations.
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
    DCMotorForward(&motor1, 90);
    DCMotorForward(&motor2, 90);
    chThdSleepMilliseconds(3000);

// FREE
    DCMotorFree(&motor1);
    DCMotorFree(&motor2);
    chThdSleepMilliseconds(1000);

// INDIETRO
    DCMotorBackward(&motor1, 70);
    DCMotorBackward(&motor2, 70);
    chThdSleepMilliseconds(2000);

// FREE
    DCMotorFree(&motor1);
    DCMotorFree(&motor2);
    chThdSleepMilliseconds(1000);

// GIRA A SINISTRA
    DCMotorTurnL(&motor1, &motor2, 70);

// FREE
    DCMotorFree(&motor1);
    DCMotorFree(&motor2);
    chThdSleepMilliseconds(1000);

// RUOTA A DESTRA
    DCMotorTurnR(&motor1, &motor2, 70);

// STOP
    DCMotorStop(&motor1);
    DCMotorStop(&motor2);

// RUOTA DI 90 GRADI  
    DCMotorSplitL(&motor1, &motor2, 90);

// FREE
    DCMotorFree(&motor1);
    DCMotorFree(&motor2);
    chThdSleepMilliseconds(1000);

    return 0;
}






