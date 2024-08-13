#ifndef __API_I2C_H
#define __API_I2C_H

#include<stdint.h>
#include<stdbool.h>
#include "stm32f4xx_hal.h"
/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
/*----------------------------------------------------------------------------*/
/*														PROTOTIPOS DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
bool i2cInit();
bool i2cSendData(uint16_t devAddress, uint8_t *pData, uint16_t size);

bool i2cReceiveData(uint16_t devAddress, uint8_t *pData, uint16_t size);
bool readi2cStatus();

#endif /*__API_I2C_H*/
