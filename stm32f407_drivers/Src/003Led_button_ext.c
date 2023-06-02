/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


#include <stdint.h>
#include "stm32f407xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define HIGH			1
#define LOW 			0
#define BTN_PRESSED		LOW

void delay(void){
	for (uint32_t i = 0; i < 500000/2; i++);
}

int main(){
	GPIO_Handle_t GpioLed, GPIOBtn;


	/* LED configuration */

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinÇonfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GpioLed.GPIO_PinÇonfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinÇonfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinÇonfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinÇonfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioLed);

	/* Button pin configuration */

	GPIOBtn.pGPIOx = GPIOB;
	GPIOBtn.GPIO_PinÇonfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIOBtn.GPIO_PinÇonfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinÇonfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GPIOBtn.GPIO_PinÇonfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOB, ENABLE);

	GPIO_Init(&GPIOBtn);

	while(1){
		if(GPIO_ReadFromInputPin(GPIOB, GPIO_PIN_NO_12) == BTN_PRESSED){
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_8);
		}


	}
}