#pragma once


#ifndef MYAPP_MYPR_MYPR_H_
#define MYAPP_MYPR_MYPR_H_

#include "main.h"
#include "arm_math.h"

typedef struct
{
    float32_t Kp;
    float32_t Kr;   //kr越大，谐振点增益越大，
    float32_t wo;   //目标角频率，单位是弧度每秒
    float32_t wc;   //wc越大，周围频带的增益也就越大
    float32_t Ts;
    float32_t A0, A1, A2, B0, B1, B2;
    float32_t vo, vo_1, vo_2;
    float32_t vi, vi_1, vi_2;
} PR;

extern PR pr_v_ac;
extern PR pr_i_ac;

void PR_init(PR *p, float32_t Kp, float32_t Kr, float32_t Ts, float32_t wc, float32_t wo);
void PR_calc(PR *p);

#endif /* MYAPP_MYPR_MYPR_H_ */

