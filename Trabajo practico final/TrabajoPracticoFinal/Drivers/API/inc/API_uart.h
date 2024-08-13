#ifndef __API_UART_H
#define __API_UART_H

/* Includes ------------------------------------------------------------------*/
#include<stdint.h>
#include<stdbool.h>
#include "API_DS1307.h"
#include "stm32f4xx_hal.h"
#include "API_delay.h"

/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
/*----------------------------------------------------------------------------*/
/*														PROTOTIPOS DE FUNCIONES 						  					*/
/*----------------------------------------------------------------------------*/
bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
bool readuartStatus();
commandType readCommand();
void bringData(timeDate_t * rtcSettings);
#endif /*__API_UART_H*/