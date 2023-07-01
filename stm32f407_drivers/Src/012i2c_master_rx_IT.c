/*
 * 010i2c_master_tx.c
 *
 *  Created on: Jun 13, 2023
 *      Author: rafae
 */


/*
 * GPIOs used as I2C pins:
 * 	- PB6 --> SCL
 * 	- PB7 --> SDA
 * Alt function mode -- > 4
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>


I2C_Handle_t I2C1Handle;

// some data
#define MAX_LEN				500
uint8_t messageLenght;
uint8_t message[MAX_LEN];

// code for length
uint8_t commandCode[] = {0x51, 0x52};

#define MY_ADDR			0x61
#define SLAVE_ADDR		0x68

// Flag variable
uint8_t rxComplt = RESET;

void delay(void){
	for (uint32_t i = 0; i < 500000/2; i++);
}

void I2C1_GPIOInit(){

	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	// SCL pin
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	// SDA pin
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);

}

void I2C1_Inits(){

	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR; // Not necessary in master mode
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = 0; // not used

	I2C_Init(&I2C1Handle);

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

	GPIO_ButtonInit();

	// I2C pin inits
	I2C1_GPIOInit();

	// I2C configuration
	I2C1_Inits();

	// I2C IRQ configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);


	// Enable the I2C1 peripheral control
	I2C1_CONTROL(ENABLE);

	// Enable the ack
	I2C_ACK_ENDI(I2C1Handle.pI2Cx, ENABLE);

	while(1){
		// Wait for the button pressed
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

		delay();

		// Send first command to get the length
		while ( I2C_MasterSendDataIT(&I2C1Handle, &commandCode[0], 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY );
		// Length stored
		while ( I2C_MasterReceiveDataIT(&I2C1Handle, &messageLenght, 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY );

		// Send second command to get the message
		while ( I2C_MasterSendDataIT(&I2C1Handle, &commandCode[1], 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY );
		// Receive the message
		while ( I2C_MasterReceiveDataIT(&I2C1Handle, message, messageLenght, SLAVE_ADDR, I2C_DISABLE_SR) != I2C_READY );

		rxComplt = RESET; // reset the value that was set by the first reception function

		// Add EOF to the new message
		while (rxComplt != SET); // Wait until reception finish
		message[messageLenght+1] = '\0';

		printf("Data received: %s", message);

		rxComplt = RESET;
	}


	return 0;
}

void I2C_ApplicationEventCallback(I2C_Handle_t *I2C1Handle, uint8_t AppEv){
	if (AppEv == I2C_EV_TX_CMPLT){
		printf("Tx is completed\n");
	}else if (AppEv == I2C_EV_RX_CMPLT){
		printf("Rx is completed\n");
		rxComplt = SET;
	}else if (AppEv == I2C_ERROR_AF){
		printf("Error: ACK failure\n");
		// In master mode ack failure happens with slave fails to send ack for the byte
		// sent from the master
		I2C_CloseSendData(I2C1Handle);

		// Generate the STOP condition to release the bus
		I2C_STOP_CONDITION(I2C1Handle->pI2Cx);

		// Hang in infinite loop to avoid other functions executes
		while(1);

	}
}

/* Runs when a data byte is received from the peripheral over I2C */
void I2C1_EV_IRQHandler(void){
	I2C_EV_IRQHandling(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void){
	I2C_ER_IRQHandling(&I2C1Handle);
}

