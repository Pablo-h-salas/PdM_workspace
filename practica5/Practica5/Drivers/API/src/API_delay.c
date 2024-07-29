#include "API_delay.h"
#include "stm32f4xx_hal.h"

/**
	*@brief carga duracion del retardo y establece flag running false
	*@param delay			: Estructura delay_t
	*@param	duration	:	Duracion del retardo en la estructura
	*/
void delayInit( delay_t * delay, tick_t duration ){
	if(delay == NULL){
		return;
	}
	if(duration == 0){
		return;
	}
	delay -> duration = duration;
	delay -> running = false;
}

/**
	*@brief 	verifica el estado del flag running
	*@param 	delay			: Estructura delay_t
	*@retval 	bool_t		: devuelve true cuando se ha cumplido con el retardo
	*/
bool_t delayRead( delay_t * delay ){
	if (delay == NULL){
		return false;
	}
	if(delay ->running){
		if(HAL_GetTick()- delay->startTime >= delay->duration){
			delay ->running = false;
			return true;
		}
		
	}else{
		delay ->startTime = HAL_GetTick();
		delay ->running = true;
	}
	return false;
}

/**
	*@brief cambia el valor de la variable 'duration' de una estructura delay_t
	*@param delay			: puntero a una estructura delay_t
	*@param duration 	: campo 'duration' de una estructura delay_t
	*/
void delayWrite( delay_t * delay, tick_t duration ){
	if(delay == NULL){
		return;
	}
	if(duration == 0){
		return;
	}
	
	delay->duration = duration;
}

/**
	*@brief: Devuelve el valor de flag running
	*@param delay		: puntero a estructura delay_t 
	*/
bool_t delayIsRunning(delay_t * delay){
	if(delay == NULL){
		return false;
	}
	return delay -> running;
}

