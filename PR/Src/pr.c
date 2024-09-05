#include "pr.h"
#include "main.h"
#include "arm_math.h"

PR pr_v_ac = { 0 };
PR pr_i_ac = { 0 };

void PR_init(PR *p,float32_t Kp,float32_t Kr,float32_t Ts,float32_t wc, float32_t wo)
{
    float temp = 0;
    p->Ts=Ts;
    p->Kp=Kp;
    p->Kr=Kr;
    p->wc=wc;
    p->wo=wo;
    temp = 4 / p->Ts / p->Ts + 4 * p->wc / p->Ts + p->wo * p->wo;

    p->B0 = (4 * p->Kp / p->Ts / p->Ts + 4 * p->wc * (p->Kp + p->Kr) / p->Ts
            + p->Kp * p->wo * p->wo) / temp;
    p->B1 = (-8 * p->Kp / p->Ts / p->Ts + 2 * p->Kp * p->wo * p->wo) / temp;
    p->B2 = (4 * p->Kp / p->Ts / p->Ts - 4 * p->wc / p->Ts * (p->Kp + p->Kr)
            + p->Kp * p->wo * p->wo) / temp;
    p->A1 = (-8 / p->Ts / p->Ts + 2 * p->wo * p->wo) / temp;
    p->A2 = (4 / p->Ts / p->Ts - 4 * p->wc / p->Ts + p->wo * p->wo) / temp;
    /*PRpr;
     * 差分方程
     * y[n]+A1[n-1]+A2[n-2]=B0x[n]+B1x[n-1]+B2[n-2]
     */
}

void PR_calc(PR *p)
{
    p->vo = -p->A1 * p->vo_1 - p->A2 * p->vo_2 + p->B0 * p->vi + p->B1 * p->vi_1
            + p->B2 * p->vi_2;

    // update and store
    p->vo_2=p->vo_1;
    p->vo_1 = p->vo;
    p->vi_2 = p->vi_1;
    p->vi_1 = p->vi;
}

