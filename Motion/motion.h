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
 void DCMotorConfig(DCMotor *);

 //comandi generici
 void DCMotorForward(DCMotor *, int );
 void DCMotorBackward(DCMotor *, int );
 void DCMotorStop(DCMotor *);
 void DCMotorFree(DCMotor *);
 //comandi per biciclo
 void DCMotorTurnR(DCMotor *, DCMotor *, int );
 void DCMotorTurnL(DCMotor *, DCMotor *, int );
 void DCMotorSplitL(DCMotor *, DCMotor *, int );
 void DCMotorSplitR(DCMotor *, DCMotor *, int );


#endif /* MOTION_H_ */
