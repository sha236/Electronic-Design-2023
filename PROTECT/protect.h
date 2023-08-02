#pragma once
#include "main.h"

#define HOLD_MOS() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,GPIO_PIN_RESET)
#define ENABLE_MOS() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,GPIO_PIN_SET)

void PROTECT_GUARD(void);