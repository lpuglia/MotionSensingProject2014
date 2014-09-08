/*
 * motion.h
 */

#ifndef MOTION_H_
#define MOTION_H_

#include "hal.h"
#include "ch.h"


//  DEFINIZIONE STRUTTURA gpiopin
typedef struct gpiopin_t{
  ioportid_t port;
  ioportmask_t pin;
} GPIO_PIN;

//  DEFINIZIONE STRUTTURA Motor
typedef struct Motor_t{
    // pin GPIO a cui e' connesso DCMotor IN1
    GPIO_PIN IN1;
    // pin GPIO a cui e' connesso DCMotor IN2
    GPIO_PIN IN2;
    // pin GPIO a cui e' connesso DCMotor EN
    GPIO_PIN EN;
    //  driver PWM utilizzato con EN
    PWMDriver *pwm_driver;
    // canale PWM utilizzato con EN
    pwmchannel_t pwm_channel;
} DCMotor;

/****************************************************************************
 *  Funzioni Motori
 ****************************************************************************/

 //init
 void DCMotorConfig(DCMotor *dcmotor);

 //comandi
 void DCMotorForward(DCMotor *dcmotor, int speedPercent);
 void DCMotorBackward(DCMotor *dcmotor, int speedPercent);
 void DCMotorStop(DCMotor *dcmotor);
 void DCMotorFree(DCMotor *dcmotor);
 void DCMotorTurnR(DCMotor *dcmotor1, DCMotor *dcmotor2, int speedPercent);
 void DCMotorTurnL(DCMotor *dcmotor1, DCMotor *dcmotor2, int speedPercent);
 void DCMotorSplitL(DCMotor *dcmotor1, DCMotor *dcmotor2, int degree);
 void DCMotorSplitR(DCMotor *dcmotor1, DCMotor *dcmotor2, int degree);


#endif /* MOTION_H_ */
