#include "API_debounce.h"
#include "API_delay.h"
#include "stm32f4xx_hal.h"

#define DEBOUNCEtIME  40
#define PORTbUTTON 	GPIOA

/*----------------------------------------------------------------------------*/
/*												DECLARACION DE VARIABLES Y TIPOS 						  			*/
/*----------------------------------------------------------------------------*/
typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RAISING,
} debounceState_t;

static debounceState_t estadoActual;
static delay_t debounceTimer;
static bool_t flag = 0; // flag en 0 = pulsador no presionado
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
			// este estado indica pulsador no presionado entonces llamo a funcion:
			buttonReleased();
			break;
		case BUTTON_FALLING:
			// consultar si luego de xx ms el pulsador sigue presionado
			if(delayRead(&debounceTimer)){
				if (HAL_GPIO_ReadPin(PORTbUTTON, button)==GPIO_PIN_SET){
				estadoActual= BUTTON_DOWN;
				flag = 1; // flag nos indica que el pulsador ha sido presionado
				}
			}
			break;
		case BUTTON_DOWN:
			//consultar si el pulsador no es presionado
			if (HAL_GPIO_ReadPin(PORTbUTTON, button)==GPIO_PIN_RESET){
				estadoActual=BUTTON_RAISING;
				delayWrite(&debounceTimer,DEBOUNCEtIME);
			}
			//este estado indica pulsador presionado entonces llamo a funcion:
			buttonPressed();
			break;
		case BUTTON_RAISING:
			//consultar si luego de xx ms el pulsador sigue no-presionado
			if(delayRead(&debounceTimer)){
				if (HAL_GPIO_ReadPin(PORTbUTTON, button)==GPIO_PIN_RESET){
					estadoActual=BUTTON_UP;
				}
			}
			break;
		default:
			// reinicio la FSM
			debounceFSM_init();
			break;
	}
}	
/**
	*@brief Devuelve true si la tecla fue presionado, caso contrario false.
					Resetea la variable.
	*@param no hay parametros a recibir
	*/
bool_t readKey(){
	if (flag){
		flag = 0;
		return true;
	}else{
		return false;
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