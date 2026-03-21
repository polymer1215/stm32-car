#include "../Inc/pid.h"

/**
 * @brief PID 初始化
 * @param mode 模式选择 (PID_MODE_POSITIONAL 或 PID_MODE_INCREMENTAL)
 * @param p, i, d 浮点 PID 参数
 * @param o_max 输出上限
 * @param o_min 输出下限
 */
void PID_Init(PID_Instance *pid, PID_Mode_t mode, float p, float i, float d, int32_t o_max, int32_t o_min) {
    pid->mode = mode;
    pid->Kp = (int32_t)(p * PID_SCALE_FACTOR);
    pid->Ki = (int32_t)(i * PID_SCALE_FACTOR);
    pid->Kd = (int32_t)(d * PID_SCALE_FACTOR);

    pid->out_max = o_max;
    pid->out_min = o_min;

    // 默认积分限幅设为输出上限的一半，可根据需要调整
    pid->int_max = o_max << 2;

    PID_Reset(pid);
}

/**
 * @brief 重置 PID 状态（切换目标或重启系统时调用）
 */
void PID_Reset(PID_Instance *pid) {
    pid->err_last = 0;
    pid->err_prev = 0;
    pid->integral = 0;
    pid->current_out = 0;
}

/**
 * @brief 通用 PID 计算
 * @param target 目标值 (int32)
 * @param measure 测量值 (int32)
 * @return 控制输出
 */
int32_t PID_Compute(PID_Instance *pid, int32_t target, int32_t measure) {
    int32_t err = target - measure;
    int32_t out = 0;

    if (pid->mode == PID_MODE_POSITIONAL) {
        /* --- 位置式 PID 逻辑 --- */
        // 积分并限幅
        pid->integral += err;
        if (pid->integral > pid->int_max) pid->integral = pid->int_max;
        else if (pid->integral < -pid->int_max) pid->integral = -pid->int_max;

        // 计算输出 (P + I + D) >> 10
        out = (pid->Kp * err +
               pid->Ki * pid->integral +
               pid->Kd * (err - pid->err_last)) >> PID_SHIFT;

        pid->err_last = err;
    }
    else {
        /* --- 增量式 PID 逻辑 --- */
        // delta_out = [Kp(e-e_l) + Ki*e + Kd(e-2e_l+e_p)] >> 10
        int32_t delta_out = (pid->Kp * (err - pid->err_last) +
                             pid->Ki * err +
                             pid->Kd * (err - 2 * pid->err_last + pid->err_prev)) >> PID_SHIFT;

        pid->current_out += delta_out;
        out = pid->current_out;

        // 状态滚动
        pid->err_prev = pid->err_last;
        pid->err_last = err;
    }

    /* 统一最终输出限幅 */
    if (out > pid->out_max) out = pid->out_max;
    else if (out < pid->out_min) out = pid->out_min;

    // 同步增量式的当前输出，防止限幅失效
    if (pid->mode == PID_MODE_INCREMENTAL) pid->current_out = out;

    return out;
}