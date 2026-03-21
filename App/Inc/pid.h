#ifndef __PID_H
#define __PID_H

#include <stdint.h>

/* 定点数缩放位移：2^10 = 1024，兼顾精度与 F103 位移效率 */
#define PID_SHIFT         10
#define PID_SCALE_FACTOR  (1 << PID_SHIFT)

typedef enum {
    PID_MODE_POSITIONAL = 0,
    PID_MODE_INCREMENTAL,
} PID_Mode_t;

typedef struct {
    PID_Mode_t mode;

    int32_t Kp;
    int32_t Ki;
    int32_t Kd;

    int32_t err_last;   // e(k-1)
    int32_t err_prev;   // e(k-2) -> 仅增量式使用
    int32_t integral;   // 误差累积 -> 仅位置式使用

    int32_t int_max;    // 积分限幅 (防止位置式积分过冲)
    int32_t out_max;    // 输出限幅 (物理限制，如 PWM 最大值)
    int32_t out_min;    // 输出限幅 (最小物理限制)

    int32_t current_out; // 当前输出值 (增量式需要持续累加)
} PID_Instance;

void PID_Init(PID_Instance *pid, PID_Mode_t mode, float p, float i, float d, int32_t o_max, int32_t o_min);
int32_t PID_Compute(PID_Instance *pid, int32_t target, int32_t measure);
void PID_Reset(PID_Instance *pid);

#endif