//
// Created by Administrator on 2026/3/18.
//

#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern volatile int32_t leftMotorDeg;
extern volatile int32_t rightMotorDeg;
extern uint8_t pidEnabled;

extern volatile int32_t leftMotorPwm;
extern volatile int32_t rightMotorPwm;

void allMotorInit();
void enablePid();
void disablePid();

void setLeftMotorDeg(int32_t deg);
void setRightMotorDeg(int32_t deg);

void updateAllMotor();
void updateRightMotorSpeed();
void updateLeftMotorSpeed();
void rightMotorPid();
void leftMotorPid();


void setLeftMotorPwm(int32_t pwm);
void setRightMotorPwm(int32_t pwm);

#ifdef __cplusplus
}
#endif

#endif //MOTOR_H
