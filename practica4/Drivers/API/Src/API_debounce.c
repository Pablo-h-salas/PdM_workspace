#include "API_debounce.h"
#include "API_delay.h"
#include "stm32f4xx_hal.h"

#define DEBOUNCEtIME  40
#define PORTbUTTON 	GPIOA

/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
debounceState_t estadoActual;
delay_t debounceTimer;
uint16_t button = GPIO_PIN_0;
/*----------------------------------------------------------------------------*/
/*												DEFINICION DE FUNCIONES  						  							*/
/*----------------------------------------------------------------------------*/

/**
	*@brief carga el estado inicial de FSM e inicializa el timer
	*@param no hay parametros a recibir
	*/
void debounceFSM_init(){
	estadoActual = BUTTON_UP;
	delayInit(&debounceTimer,DEBOUNCEtIME);
}		

/**
	*@brief Lee las entradas, realiza la logica de transicion de datos y 
					actualiza las salidas
	*@param no hay parametros a recibir
	*/
void debounceFSM_update(){
	switch(estadoActual){
		case BUTTON_UP:
			// consultar si el pulsador es presionado
			if (HAL_GPIO_ReadPin(PORTbUTTON, button)==GPIO_PIN_SET){
				estadoActual=BUTTON_FALLING;
				delayWrite(&debounceTimer,DEBOUNCEtIME);
			}
			break;
		case BUTTON_FALLING:
			if(delayRead(&debounceTimer)){
				estadoActual= BUTTON_DOWN;
			}
			break;
		case BUTTON_DOWN:
			break;
		case BUTTON_RAISING:
			break;
		default:
			break;
	}
}	

/**
	*@brief enciende LED
	*@param no hay parametros a recibir
	*/
void buttonPressed(){
}

/**
	*@brief apaga LED
	*@param no hay parametros a recibir
	*/
void buttonReleased(){
}			