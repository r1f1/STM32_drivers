/*
 * test.c
 *
 *  Created on: Jun 12, 2023
 *      Author: rafae
 */

#include "stm32f407xx.h"
#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


int main(void){

	RCC->CFGR |= (0xA << 4);

	printf("%ld\n", RCC_GetPCLK1Value());

	while(1);
}
