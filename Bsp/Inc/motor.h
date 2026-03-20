//
// Created by Administrator on 2026/3/18.
//

#ifndef MOTOR_H
#define MOTOR_Ho

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct PidTypeDef {
    float Kp;
    float Ki;
    float Kd;
    float target;
    float measure;
    float err;
    float err_last;
    float integral;
    float out_max;      // 输出限幅
    float integral_max; // 积分限幅（抗饱和）
    float out;
} PidTypeDef;

extern volatile float leftMotorDeg;
extern volatile float rightMotorDeg;
extern uint8_t pidEnabled;

extern volatile int32_t leftMotorPwm;
extern volatile int32_t rightMotorPwm;

void allMotorInit();
void enablePid();
void disablePid();

void setLeftMotorDeg(float deg);
void setRightMotorDeg(float deg);

void updateAllMotor();
void updateRightMotorSpeed();
void updateLeftMotorSpeed();
void rightMotorPid();
void leftMotorPid();


void setLeftMotorPwm(int32_t pwm);
void setRightMotorPwm(int32_t pwm);
float getPidOutput(PidTypeDef* pid, float target, float measure);

#ifdef __cplusplus
}
#endif

#endif //MOTOR_H
