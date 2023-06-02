/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: 26 may. 2023
 *      Author: rafae
 */

#include "stm32f407xx_spi_driver.h"

/*
 * Peripheral Clock setup
 */

/*************************************************************************************
 * @fn				- SPI_PeriClockControl
 *
 * @brief			- This function enables or disables peripheral clock for the given SPI port
 *
 * @param[in]		- Base address of the SPI peripheral
 * @param[in]		- ENABLE or DISABLE macros
 *
 * @return 			- none
 *
 * @Note			- none
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if (EnorDi == ENABLE) {
		if(pSPIx == SPI1) {
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2) {
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3) {
			SPI3_PCLK_EN();
		}
	}else {
		if(pSPIx == SPI1) {
			SPI1_PCLK_DI();
		}else if(pSPIx == SPI2) {
			SPI2_PCLK_DI();
		}else if(pSPIx == SPI3) {
			SPI3_PCLK_DI();
		}
	}
}

/*
 * Init and De-Init
 */

/*************************************************************************************
 * @fn				- SPI_Init
 *
 * @brief			- This function initialise the given SPI port
 *
 * @param[in]		- SPI handle
 *
 * @return 			- none
 *
 * @Note			- none
 */

/*************************************************************************************
 * @fn				- SPI_DeInit
 *
 * @brief			- This function reset values for the given SPI port
 *
 * @param[in]		- Base address of the SPI peripheral
 *
 * @return 			- none
 *
 * @Note			- none
 */


void SPI_Init(SPI_Handle_t *pSPIHandle){

	uint32_t tempReg = 0;

	/* Enable the peripheral clock */
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	/* 1. Configure Device mode */
	tempReg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	/* 2. Configure Bus type */
	if ((pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_FULL_DUPLEX) || (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_SIMPLEX_TXONLY)){
		pSPIHandle->pSPIx->CR[0] &= ~(1 << SPI_CR1_BIDIMODE);
		pSPIHandle->pSPIx->CR[0] &= ~(1 << SPI_CR1_RXONLY);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_HALF_DUPLEX_RX){
		pSPIHandle->pSPIx->CR[0] |= (1 << SPI_CR1_BIDIMODE);
		pSPIHandle->pSPIx->CR[0] &= ~(1 << SPI_CR1_BIDIOE);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_HALF_DUPLEX_TX){
		pSPIHandle->pSPIx->CR[0] |= (1 << SPI_CR1_BIDIMODE);
		pSPIHandle->pSPIx->CR[0] |= (1 << SPI_CR1_BIDIOE);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_SIMPLEX_RXONLY){
		pSPIHandle->pSPIx->CR[0] &= ~(1 << SPI_CR1_BIDIMODE);
		pSPIHandle->pSPIx->CR[0] |= (1 << SPI_CR1_RXONLY);
	}

	/* 3. Configure clock speed	 */
	tempReg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

	/* 4. Configure Data frame format */
	tempReg |= (pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

	/* 5. Configure CPOL */
	tempReg |= (pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);

	/* 5. Configure CPAH */
	tempReg |= (pSPIHandle->SPIConfig.SPI_CPAH << SPI_CR1_CPAH);

	/* 5. Configure SSM */
	tempReg |= (pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);

	pSPIHandle->pSPIx->CR[0] = tempReg;
}


void SPI_DeInit(SPI_RegDef_t *pSPIx){

	if(pSPIx == SPI1) {
		SPI1_REG_RESET();
	}else if(pSPIx == SPI2) {
		SPI2_REG_RESET();
	}else if(pSPIx == SPI3) {
		SPI3_REG_RESET();
	}
}


uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){

	uint8_t value;
	(pSPIx->SR & FlagName) ? (value = FLAG_SET) : (value = FLAG_RESET);
	return value;
}

/*************************************************************************************
 * @fn				- SPI_SendData
 *
 * @brief			- SendData function is the blocking API because the function will wait until the bytes are transmitted
 *
 * @param[in]		- Base address of the SPI peripheral
 * @param[in]		- Pointer to data
 * @param[in]		- Number of bytes to transmit
 *
 * @return 			- none
 *
 * @Note			- This is blocking call. Also called polling base code, polling type API because may hang permanently
 *
 */


void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){

		while(Len){
			/* 1. Wait until Tx buffer is empty	*/
			while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET );		/* This loop can cause the block of the code permanently due to
																				   external problems or communication problems as well */

			/* 2. Check whether the data frame format is 8bit or 16bit	*/
			if (pSPIx->CR[0] & SPI_DFF_SEL){ 			// DFF at 1 -> 16bit
				pSPIx->DR = *((uint16_t *) pTxBuffer);  // We typecast the value at 2 bytes and then dereference
				pTxBuffer += sizeof(uint16_t);
				Len--;
				Len--;
				//(uint16_t*)pTxBuffer++; // we typecast the pointer to increase the address in two bytes
			}else {										// DFF at 0 -> 8bit
				pSPIx->DR = *pTxBuffer;
				Len--;
				pTxBuffer++;
			}

		}
}

/*************************************************************************************
 * @fn				- SPI_ReceiveData
 *
 * @brief			- SendData function is the blocking API because the function will wait until the bytes are transmitted
 *
 * @param[in]		- Base address of the SPI peripheral
 * @param[in]		- Pointer to data
 * @param[in]		- Number of bytes to transmit
 *
 * @return 			- none
 *
 * @Note			- This is blocking call. Also called polling base code, polling type API because may hang permanently
 *
 */

/*void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint16_t *pRxBuffer, uint32_t Len){

	while(Len){
		// 1. Wait until Rx buffer is full
		while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET );		// This loop can cause the block of the code permanently due to
																			//   external problems or communication problems as well

		// 2. Check whether the data frame format is 8bit or 16bit
		if (pSPIx->CR[0] & SPI_DFF_SEL){ 			// DFF at 1 -> 16bit
			*pRxBuffer = pSPIx->DR;
			Len -= 2;
			pRxBuffer++;
		}else {										// DFF at 0 -> 8bit
			uint8_t temp = pSPIx->DR;
			*pRxBuffer = temp;
			Len--;
			(uint8_t*) pRxBuffer++;
		}

	}
}*/

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len)
		{
			//1. wait until RXNE is set
			while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG)  == (uint8_t)FLAG_RESET );

			//2. check the DFF bit in CR1
			if( (pSPIx->CR[0] & ( 1 << SPI_CR1_DFF) ) )
			{
				//16 bit DFF
				//1. load the data from DR to Rxbuffer address
				 *((uint16_t*)pRxBuffer) = pSPIx->DR ;
				Len--;
				Len--;
				(uint16_t*)pRxBuffer++;
			}else
			{
				//8 bit DFF
				*(pRxBuffer) = pSPIx->DR ;
				Len--;
				pRxBuffer++;
			}
		}

}



