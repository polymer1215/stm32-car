//
// Created by Administrator on 2026/3/18.
//

#include "../Inc/motor.h"
#include "tim.h"

const uint32_t RATIO = 20; // motor decceration ratio
const uint32_t FREQUENCY = 100; // tim4 interrupt frequency
const uint32_t LINE = 13;
const uint32_t REV = LINE * 4 * RATIO;
const int32_t MAX_pwm = 3599;

const int32_t KP = 5000;
// const int32_t KI = 500;
const int32_t KI = 1550;
const int32_t KD = 200;

PidTypeDef pid_left;
PidTypeDef pid_right;

uint8_t pidEnabled = 0;
int32_t leftMotorDegTarget = 0;
int32_t rightMotorDegTarget = 0;

volatile int32_t rightMotorDeg = 0;
volatile int32_t leftMotorDeg = 0;
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

void setLeftMotorDeg(int32_t deg) {
    leftMotorDegTarget = deg;
}

void setRightMotorDeg(int32_t deg) {
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

    // float rpm = (float)count * 60.0f * (float)FREQUENCY / (float)REV;
    // rightMotorDeg = rpm * 6;
    // Simplified: deg = count * 6000 * 6 / 1040 = count * 36000 / 1040 = count * 450 / 13
    rightMotorDeg = ((int32_t)count * 450) / 13;
}

void updateLeftMotorSpeed() {
    int16_t count = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    // float rpm = (float)count * 60.0f * (float)FREQUENCY / (float)REV;
    // leftMotorDeg = rpm * 6;
    leftMotorDeg = ((int32_t)count * 450) / 13;
}

void rightMotorPid() {
    if (pidEnabled) {
        int32_t pwm_output = getPidOutput(&pid_right, rightMotorDegTarget, rightMotorDeg);
        setRightMotorPwm(pwm_output);
    }
}

void leftMotorPid() {
    if (pidEnabled) {
        int32_t pwm_output = getPidOutput(&pid_left, leftMotorDegTarget, leftMotorDeg);
        setLeftMotorPwm(pwm_output);
    }
}

int32_t getPidOutput(PidTypeDef* pid, int32_t target, int32_t measure) {
    pid->target = target;
    pid->measure = measure;
    pid->err = pid->target - pid->measure;

    int32_t p_out = pid->Kp * pid->err;

    pid->integral += pid->err;
    if (pid->integral > pid->integral_max) pid->integral = pid->integral_max;
    if (pid->integral < -pid->integral_max) pid->integral = -pid->integral_max;
    int32_t i_out = pid->Ki * pid->integral;

    int32_t d_out = pid->Kd * (pid->err - pid->err_last);
    pid->err_last = pid->err;

    pid->out = (p_out + i_out + d_out) / 1000; // Scaling factor 1000

    if (pid->out > pid->out_max) pid->out = pid->out_max;
    if (pid->out < -pid->out_max) pid->out = -pid->out_max;

    return pid->out;
}