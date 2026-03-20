//
// Created by Administrator on 2026/3/18.
//

#include "../Inc/motor.h"
#include "tim.h"

const uint32_t RATIO = 20; // motor decceration ratio
const uint32_t FREQUENCY = 100; // tim4 interrupt frequency
const uint32_t LINE = 13;
const uint32_t REV = LINE * 4 * RATIO;
const uint32_t MAX_pwm = 3599;

const float KP = 5.0f;
// const float KI = 0.5f;
const float KI = 1.55f;
const float KD = 0.2f;

PidTypeDef pid_left;
PidTypeDef pid_right;

uint8_t pidEnabled = 0;
float leftMotorDegTarget = 0;
float rightMotorDegTarget = 0;

volatile float rightMotorDeg = 0;
volatile float leftMotorDeg = 0;
volatile int32_t leftMotorPwm = 0;
volatile int32_t rightMotorPwm = 0;

void allMotorInit() {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT(&htim4);

    pid_left.Kp = KP;  pid_left.Ki = KI;  pid_left.Kd = KD;
    pid_left.out_max = 3599;
    pid_left.integral_max = 1000;
    pid_right = pid_left;

    pidEnabled = 0;
}

void enablePid() {
    pidEnabled = 1;
}

void disablePid() {
    pidEnabled = 0;
}

void setLeftMotorDeg(float deg) {
    leftMotorDegTarget = deg;
}

void setRightMotorDeg(float deg) {
    rightMotorDegTarget = deg;
}

void setRightMotorPwm(int32_t pwm) {
    uint32_t pwm_val = 0;
    if (pwm >= 0) {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
        pwm_val = (uint32_t)pwm;
    } else if (pwm < 0) {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
        pwm_val = (uint32_t)(-pwm);
    }
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_val);
    rightMotorPwm = pwm;
}

void setLeftMotorPwm(int32_t pwm) {
    uint32_t pwm_val = 0;
    if (pwm >= 0) {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
        pwm_val = (uint32_t)pwm;
    } else if (pwm < 0) {
        HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
        pwm_val = (uint32_t)(-pwm);
    }
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwm_val);
    leftMotorPwm = pwm;
}


void updateAllMotor() {
    updateRightMotorSpeed();
    rightMotorPid();
    updateLeftMotorSpeed();
    leftMotorPid();
}

void updateRightMotorSpeed() {
    int16_t count = (int16_t)__HAL_TIM_GET_COUNTER(&htim1);
    __HAL_TIM_SET_COUNTER(&htim1, 0);

    float rpm = (float)count * 60.0f * (float)FREQUENCY / (float)REV;
    rightMotorDeg = rpm * 6;
}

void updateLeftMotorSpeed() {
    int16_t count = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    float rpm = (float)count * 60.0f * (float)FREQUENCY / (float)REV;
    leftMotorDeg = rpm * 6;
}

void rightMotorPid() {
    if (pidEnabled) {
        float pwm_output = getPidOutput(&pid_right, (float)rightMotorDegTarget, rightMotorDeg);
        setRightMotorPwm((int32_t)pwm_output);
    }
}

void leftMotorPid() {
    if (pidEnabled) {
        float pwm_output = getPidOutput(&pid_left, (float)leftMotorDegTarget, leftMotorDeg);
        setLeftMotorPwm((int32_t)pwm_output);
    }
}

float getPidOutput(PidTypeDef* pid, float target, float measure) {
    pid->target = target;
    pid->measure = measure;
    pid->err = pid->target - pid->measure;

    float p_out = pid->Kp * pid->err;

    pid->integral += pid->err;
    if (pid->integral > pid->integral_max) pid->integral = pid->integral_max;
    if (pid->integral < -pid->integral_max) pid->integral = -pid->integral_max;
    float i_out = pid->Ki * pid->integral;

    float d_out = pid->Kd * (pid->err - pid->err_last);
    pid->err_last = pid->err;

    pid->out = p_out + i_out + d_out;

    if (pid->out > pid->out_max) pid->out = pid->out_max;
    if (pid->out < -pid->out_max) pid->out = -pid->out_max;

    return pid->out;
}