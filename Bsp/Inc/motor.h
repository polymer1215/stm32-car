//
// Created by Administrator on 2026/3/18.
//

#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct PidTypeDef {
    int32_t Kp;
    int32_t Ki;
    int32_t Kd;
    int32_t target;
    int32_t measure;
    int32_t err;
    int32_t err_last;
    int32_t integral;
    int32_t out_max;      // 输出限幅
    int32_t integral_max; // 积分限幅（抗饱和）
    int32_t out;
} PidTypeDef;

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
int32_t getPidOutput(PidTypeDef* pid, int32_t target, int32_t measure);

#ifdef __cplusplus
}
#endif

#endif //MOTOR_H
