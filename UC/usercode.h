#pragma once
#include "main.h"
#include "arm_math.h"


uint16_t ADC1_Buf[4]={0};
// CH1:ADC1_Buf[0], CH2: ADC1_Buf[1]{有问题}, CH3: ADC1_Buf[2], CH4: ADC1_Buf[3]

uint16_t ADC3_Buf[4]={0};
// CH1:ADC3_Buf[3]{有问题}, CH2: ADC3_Buf[0], CH3: ADC3_Buf[1], CH4: ADC3_Buf[2]

float32_t ADC1_Buf_f32[4] = {0};

float32_t ADC3_Buf_f32[4] = {0};