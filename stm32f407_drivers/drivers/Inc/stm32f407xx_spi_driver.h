/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: 26 may. 2023
 *      Author: rafae
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include <stddef.h>
#include "stm32f407xx.h"

/*
 * User SPI configuration structure
 */

typedef struct{
	uint8_t SPI_DeviceMode;					/* Configure device as a Master or Salve */
	uint8_t SPI_BusConfig;					/* Communication type: full-duplex, half-duplex or simplex (Default full-duplex) */
	uint8_t SPI_SclkSpeed;					/* Serial clock speed */
	uint8_t SPI_DFF;						/* Data Frame Format: 8bits or 16bits (Default 8bits) */
	uint8_t SPI_CPOL;						/* Clock polarity (Default 0) */
	uint8_t SPI_CPAH;						/* Clock phase (Default 0) */
	uint8_t SPI_SSM;						/* Slave Select Management: Software or Hardware management */
} SPI_ConfigReg_t;

/*
 * Handle structure for SPIx peripheral
 */

typedef struct{
	SPI_RegDef_t 		*pSPIx;				/* This holds the base address of the SPI port to which the pin belongs */
	SPI_ConfigReg_t 	SPIConfig;			/* This holds SPI peripheral configuration settings */
	uint8_t 			*pTxBuffer;			/* To store the application Tx buffer address */
	uint8_t 			*pRxBuffer;			/* To store the application Rx buffer address */
	uint8_t 			TxLen;				/* To store the Tx length */
	uint8_t 			RxLen;				/* To store the Rx length */
	uint8_t 			TxState;			/* To store the Tx state */
	uint8_t 			RxState;			/* To store the Rx state */

} SPI_Handle_t;

/*
 * @SPI_DEVICE_MODE
 */

#define SPI_MODE_MASTER					1
#define SPI_MODE_SLAVE					0

/*
 * @SPI_BUS_CONFIG
 */

#define SPI_BUS_FULL_DUPLEX				0
#define SPI_BUS_HALF_DUPLEX_TX			1
#define SPI_BUS_HALF_DUPLEX_RX			2
#define SPI_BUS_SIMPLEX_TXONLY			3
#define SPI_BUS_SIMPLEX_RXONLY			4

/*
 * @SPI_SCLK_SPEED
 */

#define SPI_SCLK_SPEED_DIV2				0
#define SPI_SCLK_SPEED_DIV4				1
#define SPI_SCLK_SPEED_DIV8				2
#define SPI_SCLK_SPEED_DIV16			3
#define SPI_SCLK_SPEED_DIV32			4
#define SPI_SCLK_SPEED_DIV64			5
#define SPI_SCLK_SPEED_DIV128			6
#define SPI_SCLK_SPEED_DIV256			7

/*
 * @SPI_DFF
 */

#define SPI_DFF_8_BITS 					0
#define SPI_DFF_16_BITS					1

/*
 * @SPI_CPOL
 */

#define SPI_CPOL_LOW					0
#define SPI_CPOL_HIGH					1

/*
 * @SPI_CPAH
 */

#define SPI_CPAH_1ST_EDGE				0
#define SPI_CPAH_2ND_EDGE				1

/*
 * @SPI_SSM
 */

#define SPI_SSM_ENABLE					1
#define SPI_SSM_DISABLE					0

/*
 * SPI related status flags definition
 */

#define SPI_RXNE_FLAG					(1 << SPI_SR_RXNE)
#define SPI_TXE_FLAG					(1 << SPI_SR_TXE)
#define SPI_CHSIDE_FLAG					(1 << SPI_SR_CHSIDE)
#define SPI_UDR_FLAG					(1 << SPI_SR_UDR)
#define SPI_CRCERR_FLAG					(1 << SPI_SR_CRCERR)
#define SPI_MODF_FLAG					(1 << SPI_SR_MODF)
#define SPI_OVR_FLAG					(1 << SPI_SR_OVR)
#define SPI_BSY_FLAG					(1 << SPI_SR_BSY)
#define SPI_FRE_FLAG					(1 << SPI_SR_FRE)

/*
 * Data frame format
 */

#define SPI_DFF_SEL						(1 << SPI_CR1_DFF)

/*
 * Enable SPIx peripheral control. SPE bit
 */

#define SPI2_CONTROL_EN()				(SPI2->CR[0] |= (1 << SPI_CR1_SPE))
#define SPI2_CONTROL_DI()				(SPI2->CR[0] &= ~(1 << SPI_CR1_SPE))
#define SPI2_CONTROL(x)					do{ (x) ? (SPI2->CR[0] |= (1 << SPI_CR1_SPE)) : (SPI2->CR[0] &= ~(1 << SPI_CR1_SPE)); }while(0)

/*
 * Enable SSI register
 */

#define SPI2_SSI_ENDI(x)				do{ (x) ? (SPI2->CR[0] |= (1 << SPI_CR1_SSI)) : (SPI2->CR[0] &= ~(1 << SPI_CR1_SSI)); }while(0)

/*
 * Enable SSI register
 */

#define SPI2_SSOE_ENDI(x)				do{ (x) ? (SPI2->CR[1] |= (1 << SPI_CR2_SSOE)) : (SPI2->CR[1] &= ~(1 << SPI_CR2_SSOE)); }while(0)

/*
 * SPI applications states
 */

#define SPI_READY						0
#define SPI_BUSY_IN_RX					1
#define SPI_BUSY_IN_TX					2

/*
 * Possible SPI applications events
 */

#define SPI_EVENT_TX_CMPLT				1
#define SPI_EVENT_RX_CMPLT				2
#define SPI_EVENT_OVR_ERR				3
#define SPI_EVENT_CRC_ERR				4

/************************************************************************************************
 * 									APIs supported by this driver
 * 				For more information about the APIs check the function definition
 ************************************************************************************************/

/*
 * Peripheral Clock setup
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De-Init
 */

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data send and receive
 *
 */

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

/*
 * Data send and receive (Interrupt based API)
 *
 */

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * Check FLAG enable or disable
 */

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);

/*
 * IRQ Configuration and ISR handling
 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

/*
 * Other peripheral control APIs
 */

void SPI_ClearOVRFlag(SPI_Handle_t *pSPIHandle);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/*
 * Application Callback
 */

__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv);


#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
