/*
 * 006spi_tx_testing.c
 *
 *  Created on: 27 may. 2023
 *      Author: rafae
 */
/*
 * GPIOs used as SPI pins:
 * 	- PB12 --> SPI_NSS
 * 	- PB13 --> SPI_SCLK
 * 	- PB14 --> SPI_MISO
 * 	- PB15 --> SPI_MOSI
 * Alt function mode -- > 5
 */

#include "stm32f407xx.h"
#include <string.h>

void delay(void){
	for (uint32_t i = 0; i < 500000/2; i++);
}

void SPI2_GPIOInit(){

	GPIO_Handle_t SPIHandle;

	SPIHandle.pGPIOx = GPIOB;
	SPIHandle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIHandle.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIHandle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	SPIHandle.GPIO_PinConfig.GPIO_PinAltFunMode = 5;

	// SPI_NSS
	SPIHandle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIHandle);

	// SPI_SCLK
	SPIHandle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIHandle);

	/********************* Not used for this application ********************/
	// SPI_MISO
	/*SPIHandle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIHandle);*/

	// SPI_MOSI
	SPIHandle.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIHandle);


}

void SPI2_Inits(){

	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_FULL_DUPLEX;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; // generates a clock of 2MHz
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8_BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPAH = SPI_CPAH_1ST_EDGE;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DISABLE; // Hardware slave management enabled for NSS pin

	SPI_Init(&SPI2Handle);

}


void GPIO_ButtonInit(void) {
	GPIO_Handle_t GPIOBtn;

	/* Button pin configuration */
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);
}



int main(){

	char user_data[] = "Hello world";
	//uint8_t value = 165;

	/* Initialise button */
	GPIO_ButtonInit();

	/* Initialise GPIO pins to behave as SPI pin */
	SPI2_GPIOInit();

	/* Configure SPI2 pins */
	SPI2_Inits();

	/* Enable SSOE bit */
	SPI2_SSOE_ENDI(ENABLE);

	while(1){
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

		delay();

		/* Enable SPI2 peripheral. SPE bit */
		SPI2_CONTROL(ENABLE);

		/* Before send data we have to send the length information */
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2, &dataLen, 1);

		/* Send data */
		SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

		/* Confirm that SPI is not busy and all the data is transferred */
		while( SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG) );

		/* Disable SPI2 peripheral */
		SPI2_CONTROL(DISABLE);

	}

	return 0;
}
