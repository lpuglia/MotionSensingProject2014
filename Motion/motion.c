/*
 * motion.c
 */


#include "ch.h"
#include "hal.h"
#include "motion.h"


// STRUTTURA pwmcfg
static PWMConfig pwmcfg = {
   1000000, // 1MHz PWM clock frequency
   10000,   // PWM period 10 milliseconds
   NULL,    // no callback
   {
    // PWM channels array
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},   // Channel 0
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},   // Channel 1
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},   // Channel 2
      {PWM_OUTPUT_ACTIVE_HIGH, NULL}    // Channel 3
   },
   0,
   0
};


// Get getPwmPercentage
static int getPwmPercentage(int speedPercent) {
    int p = speedPercent > 100 ? 100 : (speedPercent < 40 ? 0 : speedPercent);
    return  p*100;
}


/****************************************************************************
 *  Funzioni Motori
 ****************************************************************************/


void DCMotorConfig(DCMotor *dcmotor) {

    /* configurazione PAL */
    palSetPadMode(dcmotor->IN1.port, dcmotor->IN1.pin, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(dcmotor->IN2.port, dcmotor->IN2.pin, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(dcmotor->EN.port, dcmotor->EN.pin, PAL_MODE_ALTERNATE(2));

    /* start PWM  */
    pwmStart(dcmotor->pwm_driver, &pwmcfg);
}

void DCMotorStop(DCMotor *dcmotor) {
    palClearPad(dcmotor->IN1.port, dcmotor->IN1.pin);
    palClearPad(dcmotor->IN2.port, dcmotor->IN2.pin);
    // set EN PWM HIGH part as FULL
    pwmEnableChannel( dcmotor->pwm_driver,
                      dcmotor->pwm_channel,
                      PWM_PERCENTAGE_TO_WIDTH(dcmotor->pwm_driver,
                                              getPwmPercentage(100))
                     );
}

void DCMotorFree(DCMotor *dcmotor) {
    palClearPad(dcmotor->IN1.port, dcmotor->IN1.pin);
    palClearPad(dcmotor->IN2.port, dcmotor->IN2.pin);
    pwmEnableChannel( dcmotor->pwm_driver,
                      dcmotor->pwm_channel,
                      PWM_PERCENTAGE_TO_WIDTH(dcmotor->pwm_driver, 0)
                     );
}

void DCMotorForward(DCMotor *dcmotor, int speedPercent) {
    palClearPad(dcmotor->IN1.port, dcmotor->IN1.pin);
    palSetPad(dcmotor->IN2.port, dcmotor->IN2.pin);
    // set speed
    pwmEnableChannel( dcmotor->pwm_driver,
                      dcmotor->pwm_channel,
                      PWM_PERCENTAGE_TO_WIDTH(dcmotor->pwm_driver,
                                              getPwmPercentage(speedPercent))
                     );
}

void DCMotorBackward(DCMotor *dcmotor, int speedPercent) {
    palSetPad(dcmotor->IN1.port, dcmotor->IN1.pin);
    palClearPad(dcmotor->IN2.port, dcmotor->IN2.pin);
    // set speed
    pwmEnableChannel( dcmotor->pwm_driver,
                      dcmotor->pwm_channel,
                      PWM_PERCENTAGE_TO_WIDTH( dcmotor->pwm_driver,
                                               getPwmPercentage(speedPercent)
                                              )
                     );
}

void DCMotorTurnL(DCMotor *dcmotor1, DCMotor *dcmotor2, int speedPercent) {

    DCMotorForward(dcmotor1, speedPercent);
    chThdSleepMilliseconds(1000);
    DCMotorStop(dcmotor2);
}

void DCMotorTurnR(DCMotor *dcmotor1, DCMotor *dcmotor2, int speedPercent) {

    DCMotorStop(dcmotor1);
    DCMotorForward(dcmotor2, speedPercent);
    chThdSleepMilliseconds(1000);
}










